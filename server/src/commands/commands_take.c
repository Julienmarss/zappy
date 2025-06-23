/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_take
*/

#include "server.h"
#include "gui_protocol.h"

static bool validate_take_request(client_t *client, char **args)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (!args[1] || !is_valid_resource_name(args[1])) {
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

static bool check_resource_availability(server_t *server, player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    if (!tile || tile->resources[resource_index] <= 0)
        return false;
    return true;
}

static void execute_take_action(server_t *server, player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    tile->resources[resource_index]--;
    player->inventory[resource_index]++;
}

void cmd_take(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char *resource_name = args[1];
    int resource_index = 0;

    if (!validate_take_request(client, args))
        return;
    if (!check_resource_availability(server, player, resource_name)) {
        network_send(client, "ko\n");
        return;
    }
    resource_index = get_resource_index_by_name(resource_name);
    execute_take_action(server, player, resource_name);
    network_send(client, "ok\n");
    gui_broadcast_resource_collect(server, player, resource_index);
    gui_broadcast_player_inventory(server, player);
}
