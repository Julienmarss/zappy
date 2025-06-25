/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_gui
*/

#include "server.h"
#include "gui_protocol.h"

static void send_initial_map_data(server_t *server, client_t *client)
{
    gui_cmd_msz(server, client, NULL);
    gui_cmd_mct(server, client, NULL);
    gui_cmd_tna(server, client, NULL);
}

static void send_initial_player_data(server_t *server)
{
    client_t *c = server->clients;

    while (c) {
        if (c->type == CLIENT_PLAYER && c->player) {
            gui_broadcast_new_player(server, c->player);
            gui_broadcast_player_position(server, c->player);
            gui_broadcast_player_level(server, c->player);
            gui_broadcast_player_inventory(server, c->player);
        }
        c = c->next;
    }
}

static void send_time_unit(server_t *server, client_t *client)
{
    gui_cmd_sgt(server, client, NULL);
}

void network_handle_graphic_connection(server_t *server, client_t *client)
{
    printf("DEBUG: Setting up graphic client connection\n");
    client->type = CLIENT_GRAPHIC;
    printf("DEBUG: Sending initial map data\n");
    send_initial_map_data(server, client);
    printf("DEBUG: Sending initial player data\n");
    send_initial_player_data(server);
    printf("DEBUG: Sending time unit\n");
    send_time_unit(server, client);
    printf("DEBUG: Graphic client setup complete\n");
}
