/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game
*/

/**
 * @file game.c
 * @brief Contient les fonctions de gestion mémoire et d'accès à la carte et aux équipes du jeu.
 */

#include "server.h"

/**
 * @brief Libère tous les œufs d'une équipe.
 *
 * @param team L'équipe dont les œufs doivent être libérés.
 */
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

/**
 * @brief Libère toutes les équipes du jeu, ainsi que leurs œufs.
 *
 * @param game Le jeu contenant les équipes.
 */
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

/**
 * @brief Libère une ligne de la carte (row de `tile_t`).
 *
 * @param row Tableau de tuiles à libérer.
 * @param width Largeur de la ligne (nombre de colonnes).
 */
static void free_map_row(tile_t *row, int width)
{
    for (int x = 0; x < width; x++)
        free(row[x].players);
    free(row);
}

/**
 * @brief Libère toute la carte du jeu.
 *
 * @param game Le jeu contenant la carte.
 */
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

/**
 * @brief Détruit entièrement la structure `game_t` (carte, équipes...).
 *
 * @param game Le jeu à détruire.
 */
void game_destroy(game_t *game)
{
    if (!game)
        return;
    free_teams(game);
    free_map(game);
    free(game);
}

/**
 * @brief Retourne un pointeur vers une tuile valide de la carte, avec gestion du débordement circulaire.
 *
 * @param game Le jeu contenant la carte.
 * @param x Coordonnée x demandée.
 * @param y Coordonnée y demandée.
 * @return Pointeur vers la tuile, ou NULL si erreur.
 */
tile_t *game_get_tile(game_t *game, int x, int y)
{
    if (!game || !game->map)
        return NULL;
    x = (x % game->width + game->width) % game->width;
    y = (y % game->height + game->height) % game->height;
    return &game->map[y][x];
}
