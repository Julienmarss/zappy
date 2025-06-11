/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI protocol implementation
*/

#include "server.h"

void gui_send_map_size(server_t *server, client_t *client)
{
    char buffer[64];

    snprintf(buffer, sizeof(buffer), "msz %d %d\n",
        server->game->width, server->game->height);
    network_send(client, buffer);
}

void gui_send_tile_content(server_t *server, client_t *client, int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    char buffer[256];

    if (!tile)
        return;
    snprintf(buffer, sizeof(buffer), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile->resources[0], tile->resources[1], tile->resources[2],
        tile->resources[3], tile->resources[4], tile->resources[5],
        tile->resources[6]);
    network_send(client, buffer);
}

void gui_send_teams(server_t *server, client_t *client)
{
    team_t *team = server->game->teams;
    char buffer[128];

    while (team) {
        snprintf(buffer, sizeof(buffer), "tna %s\n", team->name);
        network_send(client, buffer);
        team = team->next;
    }
}

void gui_send_player_position(server_t *server, client_t *client,
    player_t *player)
{
    char buffer[128];

    (void)server;
    if (!player)
        return;
    snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
    network_send(client, buffer);
}

static void broadcast_to_gui_clients(server_t *server, const char *msg)
{
    client_t *client = server->clients;

    while (client) {
        if (client->type == CLIENT_GRAPHIC)
            network_send(client, msg);
        client = client->next;
    }
}

void gui_broadcasplayer_t_action(server_t *server, player_t *player,
    const char *action)
{
    char buffer[128];

    if (!player)
        return;
    if (strcmp(action, "move") == 0 || strcmp(action, "turn") == 0) {
        snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n",
            player->id, player->x, player->y, player->orientation);
        broadcast_to_gui_clients(server, buffer);
    }
}
