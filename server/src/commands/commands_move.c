/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_move
*/

#include "server.h"

static void shift_players_left(tile_t *tile, int start_index)
{
    for (int j = start_index; j < tile->player_count - 1; j++)
        tile->players[j] = tile->players[j + 1];
    tile->players[tile->player_count - 1] = NULL;
    tile->player_count--;
}

static void remove_player_from_tile(server_t *server, player_t *player)
{
    tile_t *old_tile = game_get_tile(server->game, player->x, player->y);

    if (!old_tile)
        return;
    for (int i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == player) {
            shift_players_left(old_tile, i);
            printf("DEBUG: Removed player from tile (%d,%d)\n",
                player->x, player->y);
            break;
        }
    }
}

static void add_player_to_tile(server_t *server, player_t *player)
{
    tile_t *new_tile = game_get_tile(server->game, player->x, player->y);

    if (!new_tile || new_tile->player_count >= MAX_CLIENTS)
        return;
    new_tile->players[new_tile->player_count] = player;
    new_tile->player_count++;
    printf("DEBUG: Added player to tile (%d,%d)\n", player->x, player->y);
}

static void calculate_movement_delta(int orientation, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    switch (orientation) {
        case NORTH:
            *dy = -1;
            break;
        case SOUTH:
            *dy = 1;
            break;
        case EAST:
            *dx = 1;
            break;
        case WEST:
            *dx = -1;
            break;
    }
}

static void update_player_position(server_t *server, player_t *player,
    int dx, int dy)
{
    int old_x = player->x;
    int old_y = player->y;

    remove_player_from_tile(server, player);
    player->x = (player->x + dx + server->game->width) % server->game->width;
    player->y = (player->y + dy + server->game->height) % server->game->height;
    add_player_to_tile(server, player);
    printf("DEBUG: Player moved from (%d,%d) to (%d,%d), orientation %d\n",
        old_x, old_y, player->x, player->y, player->orientation);
}

void move_player_forward(server_t *server, player_t *player)
{
    int dx = 0;
    int dy = 0;

    calculate_movement_delta(player->orientation, &dx, &dy);
    update_player_position(server, player, dx, dy);
}

void cmd_forward(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Forward command - current pos (%d,%d)\n",
        player->x, player->y);
    move_player_forward(server, player);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "move");
}

void cmd_right(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Right command - orientation %d -> ", player->orientation);
    player->orientation = (player->orientation % 4) + 1;
    printf("%d\n", player->orientation);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "turn");
}

void cmd_left(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Left command - orientation %d -> ", player->orientation);
    player->orientation = player->orientation - 1;
    if (player->orientation < 1)
        player->orientation = 4;
    printf("%d\n", player->orientation);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "turn");
}
