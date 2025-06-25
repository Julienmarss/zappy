/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_player_commands
*/

#include "gui_protocol.h"

static player_t *find_player_by_id(server_t *server, int player_id)
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

static bool validate_player_id_arg(char **args, client_t *client)
{
    if (!args[1]) {
        gui_send_bad_parameters(client);
        return false;
    }
    if (args[1][0] != '#') {
        gui_send_bad_parameters(client);
        return false;
    }
    return true;
}

static player_t *get_player_from_args(server_t *server, client_t *client,
    char **args)
{
    int player_id = 0;
    player_t *player = NULL;

    if (!validate_player_id_arg(args, client)) {
        return NULL;
    }
    player_id = atoi(args[1] + 1);
    player = find_player_by_id(server, player_id);
    if (!player) {
        printf("DEBUG: Player ID %d not found\n", player_id);
        gui_send_bad_parameters(client);
        return NULL;
    }
    return player;
}

static void send_player_position_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
    network_send(client, response);
    printf("DEBUG: Sent player position for ID %d: (%d,%d) orient %d\n",
        player->id, player->x, player->y, player->orientation);
}

static void send_player_level_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "plv #%d %d\n",
        player->id, player->level);
    network_send(client, response);
    printf("DEBUG: Sent player level for ID %d: level %d\n",
        player->id, player->level);
}

static void send_player_inventory_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
    network_send(client, response);
    printf("DEBUG: Sent player inventory for ID %d at (%d,%d)\n",
        player->id, player->x, player->y);
}

void gui_cmd_ppo(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_position_response(client, player);
}

void gui_cmd_plv(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_level_response(client, player);
}

void gui_cmd_pin(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_inventory_response(client, player);
}
