/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game
*/

#include "server.h"

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
