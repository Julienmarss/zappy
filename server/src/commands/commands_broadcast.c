/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_broadcast
*/

#include "server.h"
#include "gui_protocol.h"

static int calculate_shortest_distance(int emitter_pos, int listener_pos,
    int world_size)
{
    int direct_distance = emitter_pos - listener_pos;
    int wrap_distance = 0;
    int half_world = world_size / 2;
    int neg_half_world = -half_world;

    if (direct_distance > half_world)
        wrap_distance = direct_distance - world_size;
    else if (direct_distance < neg_half_world)
        wrap_distance = direct_distance + world_size;
    else
        wrap_distance = direct_distance;
    return wrap_distance;
}

static int get_raw_direction_from_deltas(int dx, int dy)
{
    if (dx == 0 && dy < 0)
        return 1;
    if (dx > 0 && dy < 0)
        return 2;
    if (dx > 0 && dy == 0)
        return 3;
    if (dx > 0 && dy > 0)
        return 4;
    if (dx == 0 && dy > 0)
        return 5;
    if (dx < 0 && dy > 0)
        return 6;
    if (dx < 0 && dy == 0)
        return 7;
    if (dx < 0 && dy < 0)
        return 8;
    return 0;
}

static int adjust_direction_for_orientation(int raw_direction,
    int listener_orientation)
{
    int adjusted = raw_direction - listener_orientation + 1;

    if (adjusted <= 0)
        adjusted += 8;
    if (adjusted > 8)
        adjusted -= 8;
    return adjusted;
}

int calculate_sound_direction(server_t *server, player_t *listener,
    int emitter_x, int emitter_y)
{
    int dx = 0;
    int dy = 0;
    int raw_direction = 0;

    if (listener->x == emitter_x && listener->y == emitter_y)
        return 0;
    dx = calculate_shortest_distance(emitter_x, listener->x,
        server->game->width);
    dy = calculate_shortest_distance(emitter_y, listener->y,
        server->game->height);
    raw_direction = get_raw_direction_from_deltas(dx, dy);
    return adjust_direction_for_orientation(raw_direction,
        listener->orientation);
}

char *reconstruct_broadcast_message(char **args)
{
    static char full_message[1024] = "";
    int i = 1;

    memset(full_message, 0, sizeof(full_message));
    while (args[i]) {
        if (i > 1)
            strcat(full_message, " ");
        strcat(full_message, args[i]);
        i++;
    }
    return full_message;
}

void send_broadcast_to_listeners(server_t *server,
    player_t *emitter, char *message)
{
    client_t *current = server->clients;
    char response[1024];
    int direction = 0;

    while (current) {
        if (current->type == CLIENT_PLAYER && current->player &&
            current->player != emitter) {
            direction = calculate_sound_direction(server, current->player,
                emitter->x, emitter->y);
            snprintf(response, sizeof(response), "message %d, %s\n",
                direction, message);
            network_send(current, response);
        }
        current = current->next;
    }
}

void cmd_broadcast(server_t *server, client_t *client, char **args)
{
    player_t *emitter = client->player;
    char *message = NULL;

    if (!emitter) {
        network_send(client, "ko\n");
        return;
    }
    if (!args[1]) {
        network_send(client, "ko\n");
        return;
    }
    message = reconstruct_broadcast_message(args);
    send_broadcast_to_listeners(server, emitter, message);
    network_send(client, "ok\n");
    gui_broadcast_player_broadcast(server, emitter, message);
    printf("DEBUG: Broadcast from player %d: '%s'\n",
        emitter->id, message);
}
