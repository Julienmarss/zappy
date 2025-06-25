/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_egg
*/

#include "server.h"

static void remove_egg_from_team(team_t *team, egg_t *egg)
{
    egg_t **current = &team->eggs;

    while (*current) {
        if (*current == egg) {
            *current = egg->next;
            break;
        }
        current = &(*current)->next;
    }
}

static void remove_egg_from_tile(server_t *server, egg_t *egg)
{
    tile_t *tile = game_get_tile(server->game, egg->x, egg->y);
    egg_t **current = NULL;

    if (!tile) {
        return;
    }
    current = &tile->eggs;
    while (*current) {
        if (*current == egg) {
            *current = egg->next;
            break;
        }
        current = &(*current)->next;
    }
}

egg_t *get_available_egg_from_team(team_t *team)
{
    if (!team->eggs) {
        return NULL;
    }
    return team->eggs;
}

void cleanup_egg_resources(server_t *server, team_t *team, egg_t *egg)
{
    remove_egg_from_team(team, egg);
    remove_egg_from_tile(server, egg);
    free(egg);
}
