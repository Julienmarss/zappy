/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_create
*/

#include "server.h"

static const float RESOURCE_DENSITY[] = {
    0.5,
    0.3,
    0.15,
    0.1,
    0.1,
    0.08,
    0.05
};

static void cleanup_partial_map(tile_t **map, int allocated_rows)
{
    for (int i = 0; i < allocated_rows; i++)
        free(map[i]);
    free(map);
}

static bool allocate_map_row(tile_t **map, int y, int width)
{
    map[y] = calloc(width, sizeof(tile_t));
    if (!map[y])
        return false;
    for (int x = 0; x < width; x++)
        map[y][x].players = calloc(MAX_CLIENTS, sizeof(player_t *));
    return true;
}

static bool initialize_game_data(game_t *game, game_params_t *params)
{
    game->width = params->width;
    game->height = params->height;
    game->time_unit = params->freq;
    game->map = create_map(params->width, params->height);
    return (game->map != NULL);
}

static void spawn_single_resource_type(game_t *game, int resource_type,
    int quantity)
{
    tile_t *tile = NULL;

    for (int j = 0; j < quantity; j++) {
        tile = game_get_tile(game, rand() % game->width,
            rand() % game->height);
        if (tile)
            tile->resources[resource_type]++;
    }
}

static void spawn_initial_resources(game_t *game)
{
    int total_tiles = game->width * game->height;
    int quantity = 0;

    for (int i = 0; i < NB_RESOURCES; i++) {
        quantity = (int)(total_tiles * RESOURCE_DENSITY[i]);
        if (quantity < 1)
            quantity = 1;
        spawn_single_resource_type(game, i, quantity);
    }
    game->last_resource_spawn = get_time_microseconds();
}

tile_t **create_map(int width, int height)
{
    tile_t **map = calloc(height, sizeof(tile_t *));

    if (!map)
        return NULL;
    for (int y = 0; y < height; y++) {
        if (!allocate_map_row(map, y, width)) {
            cleanup_partial_map(map, y);
            return NULL;
        }
    }
    return map;
}

game_t *game_create(game_params_t *params)
{
    game_t *game = calloc(1, sizeof(game_t));

    if (!game)
        return NULL;
    if (!initialize_game_data(game, params)) {
        free(game);
        return NULL;
    }
    init_teams(game, params->teams, params->nb_clients);
    spawn_initial_resources(game);
    return game;
}
