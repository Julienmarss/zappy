/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game creation logic
*/

#include "server.h"

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
    game_spawn_resources(game);
    return game;
}
