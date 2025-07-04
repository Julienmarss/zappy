/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_eject_utils
*/

/**
 * @file commands_eject_utils.c
 * @brief Utilitaires pour la commande d'éjection (eject) des joueurs.
 */

#include "server.h"

/**
 * @brief Décale les joueurs dans le tableau à partir d’un index donné.
 *
 * Utilisé pour retirer un joueur d'une case sans laisser de "trou".
 *
 * @param tile La tuile contenant les joueurs.
 * @param start_index Index du joueur à retirer.
 */
static void shift_players_array(tile_t *tile, int start_index)
{
    for (int j = start_index; j < tile->player_count - 1; j++)
        tile->players[j] = tile->players[j + 1];
    tile->player_count--;
}

/**
 * @brief Supprime un joueur de sa case actuelle.
 *
 * @param server Le serveur contenant le jeu.
 * @param player Le joueur à retirer de la tuile.
 */
void remove_player_from_old_tile(server_t *server, player_t *player)
{
    tile_t *old_tile = game_get_tile(server->game, player->x, player->y);
    int i = 0;

    if (!old_tile)
        return;
    for (i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == player) {
            shift_players_array(old_tile, i);
            break;
        }
    }
}

/**
 * @brief Ajoute un joueur sur une nouvelle case.
 *
 * @param server Le serveur contenant le jeu.
 * @param player Le joueur à ajouter.
 * @param new_x Coordonnée X de la nouvelle tuile.
 * @param new_y Coordonnée Y de la nouvelle tuile.
 */
void add_player_to_new_tile(server_t *server, player_t *player,
    int new_x, int new_y)
{
    tile_t *new_tile = game_get_tile(server->game, new_x, new_y);

    if (!new_tile)
        return;
    if (new_tile->player_count >= MAX_CLIENTS)
        return;
    new_tile->players[new_tile->player_count] = player;
    new_tile->player_count++;
}

/**
 * @brief Détruit tous les œufs présents sur une case spécifique.
 *
 * @param server Le serveur contenant le jeu.
 * @param x Coordonnée X de la tuile.
 * @param y Coordonnée Y de la tuile.
 */
void destroy_eggs_on_tile(server_t *server, int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    egg_t *egg = NULL;
    egg_t *next_egg = NULL;

    if (!tile)
        return;
    egg = tile->eggs;
    while (egg) {
        next_egg = egg->next;
        free(egg);
        egg = next_egg;
    }
    tile->eggs = NULL;
}

/**
 * @brief Éjecte tous les autres joueurs présents sur la même tuile qu’un joueur.
 *
 * @param server Le serveur contenant le jeu.
 * @param ejector Le joueur qui effectue l’éjection.
 * @return Le nombre de joueurs éjectés.
 */
int eject_other_players(server_t *server, player_t *ejector)
{
    tile_t *tile = game_get_tile(server->game, ejector->x, ejector->y);
    int ejected_count = 0;
    int i = 0;

    if (!tile)
        return 0;
    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i] != ejector) {
            eject_single_player(server, ejector, tile->players[i]);
            ejected_count++;
            i--; // Corrige le décalage causé par le retrait du joueur
        }
    }
    return ejected_count;
}
