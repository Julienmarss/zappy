/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Eject command implementation - EPITECH CODING STYLE COMPLIANT
*/

#include "server.h"

static void calculate_eject_direction(int orientation, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    if (orientation == NORTH)
        *dy = -1;
    else if (orientation == SOUTH)
        *dy = 1;
    else if (orientation == EAST)
        *dx = 1;
    else if (orientation == WEST)
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
    if (orientation == WEST)
        return EAST;
    return NORTH;
}

static void remove_player_from_old_tile(server_t *server, player_t *player)
{
    tile_t *old_tile = game_get_tile(server->game, player->x, player->y);
    int i = 0;
    int j = 0;

    if (!old_tile)
        return;
    for (i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == player) {
            for (j = i; j < old_tile->player_count - 1; j++)
                old_tile->players[j] = old_tile->players[j + 1];
            old_tile->player_count--;
            break;
        }
    }
}

static void add_player_to_new_tile(server_t *server, player_t *player,
    int new_x, int new_y)
{
    tile_t *new_tile = game_get_tile(server->game, new_x, new_y);

    if (!new_tile)
        return;
    if (new_tile->player_count >= MAX_CLIENTS)
        return;
    new_tile->players[new_tile->player_count] = player;
    new_tile->player_count++;
}

static void move_player_to_position(server_t *server, player_t *player,
    int new_x, int new_y)
{
    remove_player_from_old_tile(server, player);
    player->x = new_x;
    player->y = new_y;
    add_player_to_new_tile(server, player, new_x, new_y);
}

static void send_eject_notification(player_t *target, int opposite_dir)
{
    char message[32];

    if (!target->client)
        return;
    snprintf(message, sizeof(message), "eject: %d\n", opposite_dir);
    network_send(target->client, message);
}

static void eject_single_player(server_t *server, player_t *ejector,
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
    printf("DEBUG: Player %d ejected player %d from (%d,%d) to (%d,%d)\n",
        ejector->id, target->id, ejector->x, ejector->y, new_x, new_y);
}

static void destroy_eggs_on_tile(server_t *server, int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    egg_t *egg = NULL;
    egg_t *next_egg = NULL;
    int destroyed_count = 0;

    if (!tile)
        return;
    egg = tile->eggs;
    while (egg) {
        next_egg = egg->next;
        free(egg);
        destroyed_count++;
        egg = next_egg;
    }
    tile->eggs = NULL;
    if (destroyed_count > 0) {
        printf("DEBUG: Destroyed %d eggs on tile (%d,%d)\n",
            destroyed_count, x, y);
    }
}

static int eject_other_players(server_t *server, player_t *ejector)
{
    tile_t *tile = game_get_tile(server->game, ejector->x, ejector->y);
    int ejected_count = 0;
    int i = 0;

    if (!tile)
        return 0;
    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i] != ejector) {
            eject_single_player(server, ejector, tile->players[i]);
            ejected_count++;
            i--;
        }
    }
    return ejected_count;
}

void cmd_eject(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    int ejected_count = 0;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Player %d attempting eject at (%d,%d), orient %d\n",
        player->id, player->x, player->y, player->orientation);
    ejected_count = eject_other_players(server, player);
    destroy_eggs_on_tile(server, player->x, player->y);
    network_send(client, "ok\n");
    printf("DEBUG: Player %d ejected %d players\n", player->id,
        ejected_count);
    gui_broadcast_player_action(server, player, "eject");
}
