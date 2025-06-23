/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI map commands implementation
*/

#include "gui_protocol.h"

void gui_cmd_msz(server_t *server, client_t *client, char **args)
{
    char response[MAX_GUI_RESPONSE];

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    snprintf(response, sizeof(response), "msz %d %d\n",
        server->game->width, server->game->height);
    network_send(client, response);
    printf("DEBUG: Sent map size to GUI: %dx%d\n",
        server->game->width, server->game->height);
}

static bool validate_tile_coordinates(server_t *server, int x, int y)
{
    if (x < 0 || x >= server->game->width ||
        y < 0 || y >= server->game->height) {
        printf("DEBUG: Invalid tile coordinates: (%d,%d)\n", x, y);
        return false;
    }
    return true;
}

void gui_cmd_bct(server_t *server, client_t *client, char **args)
{
    int x = 0;
    int y = 0;
    tile_t *tile = NULL;
    char response[MAX_GUI_RESPONSE];

    if (!args[1] || !args[2]) {
        gui_send_bad_parameters(client);
        return;
    }
    x = atoi(args[1]);
    y = atoi(args[2]);
    if (!validate_tile_coordinates(server, x, y)) {
        gui_send_bad_parameters(client);
        return;
    }
    tile = game_get_tile(server->game, x, y);
    if (!tile) {
        gui_send_bad_parameters(client);
        return;
    }
    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    network_send(client, response);
    printf("DEBUG: Sent tile content for (%d,%d)\n", x, y);
}

static void send_single_tile_content(server_t *server, client_t *client,
    int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    char response[MAX_GUI_RESPONSE];

    if (!tile)
        return;
    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    network_send(client, response);
}

void gui_cmd_mct(server_t *server, client_t *client, char **args)
{
    int x = 0;
    int y = 0;

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Sending all tile contents to GUI\n");
    for (y = 0; y < server->game->height; y++) {
        for (x = 0; x < server->game->width; x++) {
            send_single_tile_content(server, client, x, y);
        }
    }
    printf("DEBUG: Sent %d tile contents\n",
        server->game->width * server->game->height);
}
