/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Take command implementation
*/

#include "server.h"

static bool validate_take_request(client_t *client, char **args)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (!args[1] || !is_valid_resource_name(args[1])) {
        printf("DEBUG: Take - Invalid resource name: %s\n",
            args[1] ? args[1] : "NULL");
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

    printf("DEBUG: Tile content at (%d,%d): food=%d, linemate=%d, deraumere=%d\n",
        player->x, player->y, tile->resources[0], tile->resources[1], tile->resources[2]);
    
    if (!tile || tile->resources[resource_index] <= 0) {
        printf("DEBUG: Take - No %s available at (%d,%d) (has %d)\n",
            resource_name, player->x, player->y, 
            tile ? tile->resources[resource_index] : -1);
        return false;
    }
    printf("DEBUG: Take - Found %d %s at (%d,%d)\n",
        tile->resources[resource_index], resource_name, player->x, player->y);
    return true;
}

static void execute_take_action(server_t *server, player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    tile->resources[resource_index]--;
    player->inventory[resource_index]++;
    printf("DEBUG: Player %d took %s at (%d,%d)\n",
        player->id, resource_name, player->x, player->y);
}

void cmd_take(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char *resource_name = args[1];

    if (!validate_take_request(client, args))
        return;
    if (!check_resource_availability(server, player, resource_name)) {
        network_send(client, "ko\n");
        return;
    }
    execute_take_action(server, player, resource_name);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "take");
}
