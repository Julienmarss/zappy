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

t_tile **create_map(int width, int height)
{
    t_tile **map = calloc(height, sizeof(t_tile *));

    if (!map)
        return NULL;
    for (int y = 0; y < height; y++) {
        map[y] = calloc(width, sizeof(t_tile));
        if (!map[y]) {
            for (int i = 0; i < y; i++)
                free(map[i]);
            free(map);
            return NULL;
        }
        for (int x = 0; x < width; x++)
            map[y][x].players = calloc(MAX_CLIENTS, sizeof(t_player *));
    }
    return map;
}

t_game *game_create(int width, int height, char **teams, int nb_clients,
    int freq)
{
    t_game *game = calloc(1, sizeof(t_game));

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

static void free_teams(t_game *game)
{
    t_team *team = game->teams;
    t_team *next_team = NULL;
    t_egg *egg = NULL;
    t_egg *next_egg = NULL;

    while (team) {
        next_team = team->next;
        egg = team->eggs;
        while (egg) {
            next_egg = egg->next;
            free(egg);
            egg = next_egg;
        }
        free(team->name);
        free(team);
        team = next_team;
    }
}

void free_map(t_game *game)
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

void game_destroy(t_game *game)
{
    if (!game)
        return;
    free_teams(game);
    free_map(game);
    free(game);
}

t_tile *game_get_tile(t_game *game, int x, int y)
{
    if (!game || !game->map)
        return NULL;
    x = (x % game->width + game->width) % game->width;
    y = (y % game->height + game->height) % game->height;
    return &game->map[y][x];
}

void game_spawn_resources(t_game *game)
{
    int total_tiles = 0;
    int quantity = 0;
    t_tile *tile = NULL;

    if (!game)
        return;
    total_tiles = game->width * game->height;
    for (int i = 0; i < NB_RESOURCES; i++) {
        quantity = (int)(total_tiles * RESOURCE_DENSITY[i]);
        if (quantity < 1)
            quantity = 1;
        for (int j = 0; j < quantity; j++) {
            tile = game_get_tile(game, rand() % game->width,
                rand() % game->height);
            if (tile)
                tile->resources[i]++;
        }
    }
    game->last_resource_spawn = get_time_microseconds();
}
