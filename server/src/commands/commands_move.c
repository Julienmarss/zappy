/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Movement commands implementation - FIXED
*/

#include "server.h"

static void remove_player_from_tile(server_t *server, player_t *player)
{
    tile_t *old_tile = game_get_tile(server->game, player->x, player->y);
    
    if (!old_tile)
        return;
    
    for (int i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == player) {
            for (int j = i; j < old_tile->player_count - 1; j++) {
                old_tile->players[j] = old_tile->players[j + 1];
            }
            old_tile->players[old_tile->player_count - 1] = NULL;
            old_tile->player_count--;
            printf("DEBUG: Removed player from tile (%d,%d)\n", player->x, player->y);
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

static void move_player_forward(server_t *server, player_t *player)
{
    int old_x = player->x;
    int old_y = player->y;
    int dx = 0;
    int dy = 0;

    if (player->orientation == NORTH)
        dy = -1;
    else if (player->orientation == SOUTH)
        dy = 1;
    else if (player->orientation == EAST)
        dx = 1;
    else if (player->orientation == WEST)
        dx = -1;
    remove_player_from_tile(server, player);
    player->x = (player->x + dx + server->game->width) % server->game->width;
    player->y = (player->y + dy + server->game->height) % server->game->height;
    add_player_to_tile(server, player);
    printf("DEBUG: Player moved from (%d,%d) to (%d,%d), orientation %d\n",
           old_x, old_y, player->x, player->y, player->orientation);
}

void cmd_forward(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Forward command - current pos (%d,%d)\n", player->x, player->y);
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
