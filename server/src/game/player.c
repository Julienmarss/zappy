/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "server.h"
#include "gui_protocol.h"

player_t *player_create(team_t *team, int x, int y)
{
    player_t *player = calloc(1, sizeof(player_t));
    static int id_counter = 0;

    if (!player)
        return NULL;
    id_counter++;
    player->id = id_counter;
    player->x = x;
    player->y = y;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    player->inventory[FOOD] = 10;
    player->food_timer = 126;
    player->team = team;
    player->is_incanting = false;
    player->action_time = 0;
    printf("DEBUG: Created player ID %d (%d,%d), orientation %d, lvl %d\n",
        player->id, x, y, player->orientation, player->level);
    return player;
}

void player_add_to_tile(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    if (!tile || tile->player_count >= MAX_CLIENTS) {
        printf("DEBUG: Failed to add player to tile (%d,%d)\n",
            player->x, player->y);
        return;
    }
    tile->players[tile->player_count] = player;
    tile->player_count++;
    printf("DEBUG: Added player ID %d to tile (%d,%d), count: %d\n",
        player->id, player->x, player->y, tile->player_count);
}

static void remove_player_from_tile(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int i = 0;
    int j = 0;

    if (!tile)
        return;
    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i] == player) {
            for (j = i; j < tile->player_count - 1; j++)
                tile->players[j] = tile->players[j + 1];
            tile->player_count--;
            break;
        }
    }
}

void player_destroy(player_t *player)
{
    if (!player)
        return;
    if (player->team)
        player->team->connected_clients--;
    printf("DEBUG: Destroyed player ID %d\n", player->id);
    free(player);
}

void player_die(server_t *server, player_t *player)
{
    client_t *client = NULL;

    if (!player)
        return;
    printf("DEBUG: Player ID %d is dying\n", player->id);
    gui_broadcast_player_death(server, player);
    remove_player_from_tile(server, player);
    for (client = server->clients; client; client = client->next) {
        if (client->player == player) {
            network_send(client, "dead\n");
            client->player = NULL;
            break;
        }
    }
    player_destroy(player);
}

void player_update(server_t *server, player_t *player)
{
    (void)server;
    (void)player;
}
