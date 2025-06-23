/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_eject
*/

#include "server.h"

static void calculate_eject_direction(int orientation, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    if (orientation == NORTH) {
        *dy = -1;
        return;
    }
    if (orientation == SOUTH) {
        *dy = 1;
        return;
    }
    if (orientation == EAST) {
        *dx = 1;
        return;
    }
    if (orientation == WEST)
        *dx = -1;
}

static int get_opposite_direction(int orientation)
{
    if (orientation == NORTH)
        return SOUTH;
    if (orientation == SOUTH)
        return NORTH;
    if (orientation == EAST)
        return WEST;
    return EAST;
}

static void send_eject_notification(player_t *target, int opposite_dir)
{
    char message[32];

    if (!target->client)
        return;
    snprintf(message, sizeof(message), "eject: %d\n", opposite_dir);
    network_send(target->client, message);
}

static void move_player_to_position(server_t *server, player_t *player,
    int new_x, int new_y)
{
    remove_player_from_old_tile(server, player);
    player->x = new_x;
    player->y = new_y;
    add_player_to_new_tile(server, player, new_x, new_y);
}

void eject_single_player(server_t *server, player_t *ejector,
    player_t *target)
{
    int dx = 0;
    int dy = 0;
    int new_x = 0;
    int new_y = 0;
    int opposite_dir = 0;

    calculate_eject_direction(ejector->orientation, &dx, &dy);
    new_x = (target->x + dx + server->game->width) % server->game->width;
    new_y = (target->y + dy + server->game->height) % server->game->height;
    opposite_dir = get_opposite_direction(ejector->orientation);
    move_player_to_position(server, target, new_x, new_y);
    send_eject_notification(target, opposite_dir);
}

void cmd_eject(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    eject_other_players(server, player);
    destroy_eggs_on_tile(server, player->x, player->y);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "eject");
}
