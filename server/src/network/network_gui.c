/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_gui
*/

#include "server.h"

static void send_all_tiles(server_t *server, client_t *client)
{
    int x = 0;
    int y = 0;

    for (y = 0; y < server->game->height; y++) {
        for (x = 0; x < server->game->width; x++) {
            gui_send_tile_content(server, client, x, y);
        }
    }
}

static void send_all_players(server_t *server, client_t *client)
{
    client_t *c = server->clients;

    while (c) {
        if (c->type == CLIENT_PLAYER && c->player)
            gui_send_player_position(server, client, c->player);
        c = c->next;
    }
}

void network_handle_graphic_connection(server_t *server, client_t *client)
{
    client->type = CLIENT_GRAPHIC;
    gui_send_map_size(server, client);
    gui_send_teams(server, client);
    send_all_tiles(server, client);
    send_all_players(server, client);
}
