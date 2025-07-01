/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_teams
*/

/**
 * @file game_teams.c
 * @brief Gère la création et l'initialisation des équipes et de leurs œufs dans le jeu.
 */

#include "server.h"

/**
 * @brief Crée un nouvel œuf pour une équipe donnée.
 *
 * L'œuf est assigné à une position aléatoire sur la carte et reçoit un identifiant unique.
 *
 * @param game Instance du jeu.
 * @param team Équipe pour laquelle l'œuf est créé.
 * @return Pointeur vers l'œuf créé, ou NULL en cas d'échec.
 */
static egg_t *create_egg(game_t *game, team_t *team)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg)
        return NULL;
    egg->id = game->egg_id_counter;
    game->egg_id_counter++;
    egg->x = rand() % game->width;
    egg->y = rand() % game->height;
    egg->team = team;
    egg->next = NULL;
    printf("DEBUG: Created egg ID %d at (%d,%d) for team %s\n",
        egg->id, egg->x, egg->y, team->name);
    return egg;
}

/**
 * @brief Ajoute un œuf à la tuile correspondante sur la carte.
 *
 * Parcourt la liste chaînée des œufs présents sur la tuile et ajoute l'œuf à la fin.
 *
 * @param game Instance du jeu.
 * @param egg Œuf à ajouter.
 */
static void add_egg_to_tile(game_t *game, egg_t *egg)
{
    tile_t *tile = game_get_tile(game, egg->x, egg->y);
    egg_t **current = NULL;

    if (!tile) {
        printf("DEBUG: Failed to get tile at (%d,%d)\n", egg->x, egg->y);
        return;
    }
    current = &tile->eggs;
    while (*current)
        current = &(*current)->next;
    *current = egg;
    printf("DEBUG: Added egg ID %d to tile (%d,%d)\n",
        egg->id, egg->x, egg->y);
}

/**
 * @brief Crée une nouvelle équipe avec un nom et une capacité maximale de clients.
 *
 * Alloue et initialise la structure d'équipe, puis duplique le nom de l'équipe.
 *
 * @param name Nom de l'équipe.
 * @param nb_clients Nombre maximum de clients dans l'équipe.
 * @return Pointeur vers l'équipe créée, ou NULL en cas d'échec.
 */
static team_t *create_team(const char *name, int nb_clients)
{
    team_t *team = malloc(sizeof(team_t));

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
    team->next = NULL;
    printf("DEBUG: Created team '%s' with max %d clients\n",
        name, nb_clients);
    return team;
}

/**
 * @brief Crée les œufs initiaux pour une équipe.
 *
 * Pour chaque client maximum dans l'équipe, un œuf est créé, ajouté à l'équipe et assigné à une tuile.
 *
 * @param game Instance du jeu.
 * @param team L'équipe concernée.
 * @param nb_clients Nombre d'œufs à créer (équivalent au nombre maximum de clients).
 */
static void create_team_eggs(game_t *game, team_t *team, int nb_clients)
{
    egg_t *egg = NULL;

    printf("DEBUG: Creating %d eggs for team %s\n", nb_clients, team->name);
    for (int j = 0; j < nb_clients; j++) {
        egg = create_egg(game, team);
        if (!egg) {
            printf("DEBUG: Failed to create egg %d for team %s\n",
                j, team->name);
            continue;
        }
        egg->next = team->eggs;
        team->eggs = egg;
        add_egg_to_tile(game, egg);
    }
    printf("DEBUG: Team %s now has eggs: %p\n",
        team->name, (void *)team->eggs);
}

/**
 * @brief Initialise les équipes du jeu.
 *
 * Pour chaque nom d'équipe fourni, crée une équipe, initialise ses œufs et les chaîne dans la liste des équipes du jeu.
 *
 * @param game Instance du jeu.
 * @param team_names Tableau de chaînes contenant les noms des équipes (terminé par NULL).
 * @param nb_clients Nombre maximum de clients par équipe.
 */
void init_teams(game_t *game, char **team_names, int nb_clients)
{
    team_t *team = NULL;
    team_t *last = NULL;

    printf("DEBUG: Initializing teams with %d clients each\n", nb_clients);
    for (int i = 0; team_names[i]; i++) {
        team = create_team(team_names[i], nb_clients);
        if (!team) {
            printf("DEBUG: Failed to create team %s\n", team_names[i]);
            continue;
        }
        create_team_eggs(game, team, nb_clients);
        if (last)
            last->next = team;
        else
            game->teams = team;
        last = team;
    }
    printf("DEBUG: Teams initialization complete\n");
}
