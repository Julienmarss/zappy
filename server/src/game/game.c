/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game logic implementation
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

static bool initialize_game_data(game_t *game, int width, int height,
    int freq)
{
    game->width = width;
    game->height = height;
    game->time_unit = freq;
    game->map = create_map(width, height);
    return (game->map != NULL);
}

game_t *game_create(int width, int height, char **teams, int nb_clients,
    int freq)
{
    game_t *game = calloc(1, sizeof(game_t));

    if (!game)
        return NULL;
    if (!initialize_game_data(game, width, height, freq)) {
        free(game);
        return NULL;
    }
    init_teams(game, teams, nb_clients);
    game_spawn_resources(game);
    return game;
}

static void free_team_eggs(team_t *team)
{
    egg_t *egg = team->eggs;
    egg_t *next_egg = NULL;

    while (egg) {
        next_egg = egg->next;
        free(egg);
        egg = next_egg;
    }
}

static void free_teams(game_t *game)
{
    team_t *team = game->teams;
    team_t *next_team = NULL;

    while (team) {
        next_team = team->next;
        free_team_eggs(team);
        free(team->name);
        free(team);
        team = next_team;
    }
}

static void free_map_row(tile_t *row, int width)
{
    for (int x = 0; x < width; x++)
        free(row[x].players);
    free(row);
}

void free_map(game_t *game)
{
    if (!game->map)
        return;
    for (int y = 0; y < game->height; y++) {
        if (game->map[y])
            free_map_row(game->map[y], game->width);
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

tile_t *game_get_tile(game_t *game, int x, int y)
{
    if (!game || !game->map)
        return NULL;
    x = (x % game->width + game->width) % game->width;
    y = (y % game->height + game->height) % game->height;
    return &game->map[y][x];
}
