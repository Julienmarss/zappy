/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_resources
*/

#include "server.h"
#include "gui_protocol.h"

static const float RESOURCE_DENSITY[] = {
    0.5,
    0.3,
    0.15,
    0.1,
    0.1,
    0.08,
    0.05
};

static int count_resources_in_tile(tile_t *tile, int resource_type)
{
    if (tile)
        return tile->resources[resource_type];
    return 0;
}

static int count_total_resources_on_map(game_t *game, int resource_type)
{
    int total = 0;
    tile_t *tile = NULL;

    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            tile = game_get_tile(game, x, y);
            total += count_resources_in_tile(tile, resource_type);
        }
    }
    return total;
}

static int calculate_target_quantity(game_t *game, int resource_type)
{
    int total_tiles = game->width * game->height;
    int target_quantity = (int)(total_tiles * RESOURCE_DENSITY[resource_type]);

    if (target_quantity < 1)
        target_quantity = 1;
    return target_quantity;
}

static void send_tile_update(server_t *server, int x, int y, tile_t *tile)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message),
        "bct %d %d %d %d %d %d %d %d %d\n", x, y,
        tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    gui_send_to_all_graphic_clients(server, message);
}

static void spawn_single_resource(server_t *server, int resource_type)
{
    int x = rand() % server->game->width;
    int y = rand() % server->game->height;
    tile_t *tile = game_get_tile(server->game, x, y);

    if (tile) {
        tile->resources[resource_type]++;
        send_tile_update(server, x, y, tile);
    }
}

static void spawn_resource_type(server_t *server, int resource_type,
    int quantity)
{
    int spawned = 0;

    for (int j = 0; j < quantity; j++) {
        spawn_single_resource(server, resource_type);
        spawned++;
    }
    if (spawned > 0) {
        printf("DEBUG: Spawned %d units of resource type %d\n",
            spawned, resource_type);
    }
}

static void maintain_resource_density(server_t *server, int resource_type)
{
    int current_total = count_total_resources_on_map(server->game,
        resource_type);
    int target_total = calculate_target_quantity(server->game, resource_type);
    int to_spawn = target_total - current_total;
    int spawn_amount = 0;

    if (to_spawn > 0) {
        spawn_amount = (to_spawn + 9) / 10;
        if (spawn_amount < 1)
            spawn_amount = 1;
        spawn_resource_type(server, resource_type, spawn_amount);
    }
}

void game_spawn_resources(server_t *server)
{
    if (!server || !server->game)
        return;
    for (int i = 0; i < NB_RESOURCES; i++)
        maintain_resource_density(server, i);
    server->game->last_resource_spawn = get_time_microseconds();
}
