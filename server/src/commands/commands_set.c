/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_set
*/

#include "server.h"

static bool validate_set_request(client_t *client, char **args)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (!args[1] || !is_valid_resource_name(args[1])) {
        printf("DEBUG: Set - Invalid resource name: %s\n",
            args[1] ? args[1] : "NULL");
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

static bool check_inventory_availability(player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);

    if (player->inventory[resource_index] <= 0) {
        printf("DEBUG: Set - Player %d has no %s in inventory\n",
            player->id, resource_name);
        return false;
    }
    return true;
}

static void execute_set_action(server_t *server, player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    player->inventory[resource_index]--;
    tile->resources[resource_index]++;
    printf("DEBUG: Player %d set %s at (%d,%d)\n",
        player->id, resource_name, player->x, player->y);
}

void cmd_set(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char *resource_name = args[1];

    if (!validate_set_request(client, args))
        return;
    if (!check_inventory_availability(player, resource_name)) {
        network_send(client, "ko\n");
        return;
    }
    execute_set_action(server, player, resource_name);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "set");
}
