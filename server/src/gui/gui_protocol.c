/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI protocol implementation
*/

#include "server.h"

void gui_send_map_size(t_server *server, t_client *client)
{
    char buffer[64];

    snprintf(buffer, sizeof(buffer), "msz %d %d\n",
        server->game->width, server->game->height);
    network_send(client, buffer);
}

void gui_send_tile_content(t_server *server, t_client *client, int x, int y)
{
    t_tile *tile = game_get_tile(server->game, x, y);
    char buffer[256];

    if (!tile)
        return;
    snprintf(buffer, sizeof(buffer), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile->resources[0], tile->resources[1], tile->resources[2],
        tile->resources[3], tile->resources[4], tile->resources[5],
        tile->resources[6]);
    network_send(client, buffer);
}

void gui_send_teams(t_server *server, t_client *client)
{
    t_team *team = server->game->teams;
    char buffer[128];

    while (team) {
        snprintf(buffer, sizeof(buffer), "tna %s\n", team->name);
        network_send(client, buffer);
        team = team->next;
    }
}

void gui_send_player_position(t_server *server, t_client *client,
    t_player *player)
{
    char buffer[128];

    (void)server;
    if (!player)
        return;
    snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
    network_send(client, buffer);
}

static void broadcast_to_gui_clients(t_server *server, const char *msg)
{
    t_client *client = server->clients;

    while (client) {
        if (client->type == CLIENT_GRAPHIC)
            network_send(client, msg);
        client = client->next;
    }
}

void gui_broadcast_player_action(t_server *server, t_player *player,
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
