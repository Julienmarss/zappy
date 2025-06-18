/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_resources
*/

#include "server.h"

static const float RESOURCE_DENSITY[] = {
    0.05,
    0.03,
    0.015,
    0.01,
    0.01,
    0.008,
    0.005
};

static void spawn_resource_type(game_t *game, int resource_type, int quantity)
{
    tile_t *tile = NULL;

    for (int j = 0; j < quantity; j++) {
        tile = game_get_tile(game, rand() % game->width,
            rand() % game->height);
        if (tile)
            tile->resources[resource_type]++;
    }
}

static int calculate_resource_quantity(game_t *game, int resource_type)
{
    int total_tiles = game->width * game->height;
    int quantity = (int)(total_tiles * RESOURCE_DENSITY[resource_type]);

    if (quantity < 1)
        quantity = 1;
    return quantity;
}

void game_spawn_resources(game_t *game)
{
    int quantity = 0;

    if (!game)
        return;
    for (int i = 0; i < NB_RESOURCES; i++) {
        quantity = calculate_resource_quantity(game, i);
        spawn_resource_type(game, i, quantity);
    }
    game->last_resource_spawn = get_time_microseconds();
}
