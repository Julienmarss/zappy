/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI utility functions
*/

#include "gui.h"

void gui_send_to_all_graphic_clients(server_t *server, const char *message)
{
    client_t *client = server->clients;
    int sent_count = 0;

    if (!message) {
        printf("DEBUG: Cannot send message - message is NULL\n");
        return;
    }
    while (client) {
        if (client->type == CLIENT_GRAPHIC) {
            network_send(client, message);
            sent_count++;
        }
        client = client->next;
    }
    if (sent_count > 0) {
        printf("DEBUG: Sent GUI message to %d clients: %.50s%s\n",
            sent_count, message, strlen(message) > 50 ? "..." : "");
    }
}

char *gui_format_player_id(int player_id)
{
    static char buffer[16];

    snprintf(buffer, sizeof(buffer), "#%d", player_id);
    return buffer;
}

char *gui_format_egg_id(int egg_id)
{
    static char buffer[16];

    snprintf(buffer, sizeof(buffer), "#%d", egg_id);
    return buffer;
}

bool gui_is_valid_coordinates(server_t *server, int x, int y)
{
    if (!server || !server->game)
        return false;
    if (x < 0 || x >= server->game->width)
        return false;
    if (y < 0 || y >= server->game->height)
        return false;
    return true;
}

static player_t *find_player_in_clients(server_t *server, int player_id)
{
    client_t *client = server->clients;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player &&
            client->player->id == player_id) {
            return client->player;
        }
        client = client->next;
    }
    return NULL;
}

bool gui_is_valid_player_id(server_t *server, int player_id)
{
    if (!server || player_id <= 0)
        return false;
    return find_player_in_clients(server, player_id) != NULL;
}
