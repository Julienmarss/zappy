/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_egg
*/

/**
 * @file network_egg.c
 * @brief Gestion des œufs (eggs) dans le serveur Zappy.
 */

#include "server.h"

/**
 * @brief Supprime un œuf de la liste des œufs d'une équipe.
 *
 * @param team Pointeur vers l’équipe.
 * @param egg Pointeur vers l’œuf à supprimer.
 */
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

/**
 * @brief Supprime un œuf de la liste des œufs présents sur une case de la map.
 *
 * @param server Pointeur vers le serveur.
 * @param egg Pointeur vers l’œuf à supprimer.
 */
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

/**
 * @brief Récupère un œuf disponible dans l’équipe.
 *
 * @param team Pointeur vers l’équipe.
 * @return Pointeur vers un œuf disponible ou NULL si aucun œuf n’est présent.
 */
egg_t *get_available_egg_from_team(team_t *team)
{
    if (!team->eggs) {
        return NULL;
    }
    return team->eggs;
}

/**
 * @brief Supprime un œuf de toutes les structures où il est référencé et libère sa mémoire.
 *
 * @param server Pointeur vers le serveur.
 * @param team Pointeur vers l’équipe à laquelle appartient l’œuf.
 * @param egg Pointeur vers l’œuf à nettoyer.
 */
void cleanup_egg_resources(server_t *server, team_t *team, egg_t *egg)
{
    remove_egg_from_team(team, egg);
    remove_egg_from_tile(server, egg);
    free(egg);
}
