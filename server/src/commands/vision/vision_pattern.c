/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_pattern
*/

/**
 * @file vision_pattern.c
 * @brief Définit le motif de vision utilisé pour remplir les coordonnées locales des cases visibles.
 */

#include "vision.h"

/**
 * @brief Ajoute une case à la structure de données de vision.
 *
 * @param data Structure de données de vision.
 * @param index Pointeur vers l'index courant à remplir.
 * @param x Coordonnée X locale relative au joueur.
 * @param y Coordonnée Y locale relative au joueur.
 */
static void add_tile(vision_data_t *data, int *index, int x, int y)
{
    if (*index >= data->tile_count)
        return;
    data->tiles[*index].x = x;
    data->tiles[*index].y = y;
    data->tiles[*index].index = *index;
    (*index)++;
}

/**
 * @brief Ajoute une ligne complète de vision (en largeur croissante) au niveau donné.
 *
 * @param data Structure contenant les tiles.
 * @param index Pointeur vers l'index courant.
 * @param row Ligne de profondeur (distance depuis le joueur).
 */
static void add_row(vision_data_t *data, int *index, int row)
{
    for (int col = -row; col <= row; col++)
        add_tile(data, index, col, -row);
}

/**
 * @brief Construit l’ensemble du motif de vision en fonction du niveau du joueur.
 *
 * Le motif est une pyramide de coordonnées centrée sur (0,0), qui s’élargit avec le niveau.
 *
 * @param data Structure à remplir.
 */
static void build_pattern(vision_data_t *data)
{
    int index = 0;

    add_tile(data, &index, 0, 0);
    for (int row = 1; row <= data->level; row++)
        add_row(data, &index, row);
}

/**
 * @brief Calcule le motif de vision pour un joueur donné.
 *
 * Remplit la structure `vision_data_t` avec les coordonnées locales des cases visibles.
 *
 * @param data Structure à compléter (doit déjà être allouée et initialisée).
 */
void vision_calculate_tiles(vision_data_t *data)
{
    if (!data)
        return;
    build_pattern(data);
}
