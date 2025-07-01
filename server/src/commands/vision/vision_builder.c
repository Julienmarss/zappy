/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_builder
*/

/**
 * @file vision_builder.c
 * @brief Gère l'allocation, l'initialisation et la destruction de la structure de vision d’un joueur.
 */

#include "vision.h"

/**
 * @brief Calcule le nombre total de cases visibles selon le niveau du joueur.
 *
 * Le nombre de cases est déterminé par une pyramide de vision (1 + 3 + 5 + ... jusqu’au niveau donné).
 *
 * @param level Le niveau du joueur (1 à 8).
 * @return Le nombre total de cases visibles.
 */
static int calculate_tile_count(int level)
{
    int total = 0;

    if (level < 1 || level > 8)
        level = 1;
    for (int row = 0; row <= level; row++)
        total += (2 * row + 1);
    return total;
}

/**
 * @brief Alloue dynamiquement la structure vision_data_t ainsi que son tableau de tiles.
 *
 * @param level Le niveau du joueur.
 * @return Un pointeur vers la structure allouée, ou NULL en cas d’échec.
 */
static vision_data_t *allocate_vision_data(int level)
{
    vision_data_t *data = malloc(sizeof(vision_data_t));
    int tile_count = 0;

    if (!data)
        return NULL;
    tile_count = calculate_tile_count(level);
    data->tiles = malloc(sizeof(vision_tile_t) * tile_count);
    if (!data->tiles) {
        free(data);
        return NULL;
    }
    return data;
}

/**
 * @brief Initialise la structure vision_data_t avec le niveau et le nombre de cases calculé.
 *
 * @param data Le pointeur vers la structure à initialiser.
 * @param level Le niveau du joueur.
 */
static void init_vision_data(vision_data_t *data, int level)
{
    if (level < 1 || level > 8)
        level = 1;
    data->tile_count = calculate_tile_count(level);
    data->level = level;
}

/**
 * @brief Crée une nouvelle structure de vision pour un joueur donné.
 *
 * @param level Le niveau du joueur.
 * @return Un pointeur vers la structure initialisée, ou NULL si erreur.
 */
vision_data_t *vision_create_data(int level)
{
    vision_data_t *data = allocate_vision_data(level);

    if (!data)
        return NULL;
    init_vision_data(data, level);
    return data;
}

/**
 * @brief Libère la mémoire allouée pour une structure vision_data_t.
 *
 * @param data Le pointeur vers la structure à détruire.
 */
void vision_destroy_data(vision_data_t *data)
{
    if (!data)
        return;
    if (data->tiles)
        free(data->tiles);
    free(data);
}
