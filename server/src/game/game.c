/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game logic implementation
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

tile_t **create_map(int width, int height)
{
    tile_t **map = calloc(height, sizeof(tile_t *));

    if (!map)
        return NULL;
    for (int y = 0; y < height; y++) {
        map[y] = calloc(width, sizeof(tile_t));
        if (!map[y]) {
            for (int i = 0; i < y; i++)
                free(map[i]);
            free(map);
            return NULL;
        }
        for (int x = 0; x < width; x++)
            map[y][x].players = calloc(MAX_CLIENTS, sizeof(player_t *));
    }
    return map;
}

game_t *game_create(int width, int height, char **teams, int nb_clients,
    int freq)
{
    game_t *game = calloc(1, sizeof(game_t));

    if (!game)
        return NULL;
    game->width = width;
    game->height = height;
    game->time_unit = freq;
    game->map = create_map(width, height);
    if (!game->map) {
        free(game);
        return NULL;
    }
    init_teams(game, teams, nb_clients);
    game_spawn_resources(game);
    return game;
}

static void free_teams(game_t *game)
{
    team_t *team = game->teams;
    team_t *nexteam_t = NULL;
    egg_t *egg = NULL;
    egg_t *nexegg_t = NULL;

    while (team) {
        nexteam_t = team->next;
        egg = team->eggs;
        while (egg) {
            nexegg_t = egg->next;
            free(egg);
            egg = nexegg_t;
        }
        free(team->name);
        free(team);
        team = nexteam_t;
    }
}

void free_map(game_t *game)
{
    if (!game->map)
        return;
    for (int y = 0; y < game->height; y++) {
        if (game->map[y]) {
            for (int x = 0; x < game->width; x++)
                free(game->map[y][x].players);
            free(game->map[y]);
        }
    }
    free(game->map);
}

void game_destroy(game_t *game)
{
    if (!game)
        return;
    free_teams(game);
    free_map(game);
    free(game);
}

tile_t *game_getile_t(game_t *game, int x, int y)
{
    if (!game || !game->map)
        return NULL;
    x = (x % game->width + game->width) % game->width;
    y = (y % game->height + game->height) % game->height;
    return &game->map[y][x];
}

void game_spawn_resources(game_t *game)
{
    int total_tiles = 0;
    int quantity = 0;
    tile_t *tile = NULL;

    if (!game)
        return;
    total_tiles = game->width * game->height;
    for (int i = 0; i < NB_RESOURCES; i++) {
        quantity = (int)(total_tiles * RESOURCE_DENSITY[i]);
        if (quantity < 1)
            quantity = 1;
        for (int j = 0; j < quantity; j++) {
            tile = game_getile_t(game, rand() % game->width,
                rand() % game->height);
            if (tile)
                tile->resources[i]++;
        }
    }
    game->last_resource_spawn = get_time_microseconds();
}
