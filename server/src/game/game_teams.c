/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Teams management
*/

#include "server.h"

static t_egg *create_egg(t_game *game, t_team *team)
{
    t_egg *egg = calloc(1, sizeof(t_egg));

    if (!egg)
        return NULL;
    egg->id = game->egg_id_counter++;
    egg->x = rand() % game->width;
    egg->y = rand() % game->height;
    egg->team = team;
    return egg;
}

static void add_egg_to_tile(t_game *game, t_egg *egg)
{
    t_tile *tile = game_get_tile(game, egg->x, egg->y);
    t_egg **current = NULL;

    if (!tile)
        return;
    current = &tile->eggs;
    while (*current)
        current = &(*current)->next;
    *current = egg;
}

static t_team *create_team(const char *name, int nb_clients)
{
    t_team *team = calloc(1, sizeof(t_team));

    if (!team)
        return NULL;
    team->name = strdup(name);
    if (!team->name) {
        free(team);
        return NULL;
    }
    team->max_clients = nb_clients;
    team->connected_clients = 0;
    team->eggs = NULL;
    return team;
}

static void create_team_eggs(t_game *game, t_team *team, int nb_clients)
{
    t_egg *egg = NULL;

    for (int j = 0; j < nb_clients; j++) {
        egg = create_egg(game, team);
        if (!egg)
            continue;
        egg->next = team->eggs;
        team->eggs = egg;
        add_egg_to_tile(game, egg);
    }
}

void init_teams(t_game *game, char **team_names, int nb_clients)
{
    t_team *team = NULL;
    t_team *last = NULL;

    for (int i = 0; team_names[i]; i++) {
        team = create_team(team_names[i], nb_clients);
        if (!team)
            continue;
        create_team_eggs(game, team, nb_clients);
        if (last)
            last->next = team;
        else
            game->teams = team;
        last = team;
    }
}
