/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_coordinates
*/

/**
 * @file vision_coordinates.c
 * @brief Gère la transformation des coordonnées de vision selon l’orientation du joueur.
 */

#include "vision.h"

/**
 * @brief Transformation de coordonnées pour une orientation nord.
 *
 * @param dx Position locale X.
 * @param dy Position locale Y.
 * @param wx Pointeur vers la coordonnée X dans le monde.
 * @param wy Pointeur vers la coordonnée Y dans le monde.
 */
static void transform_north(int dx, int dy, int *wx, int *wy)
{
    *wx = dx;
    *wy = dy;
}

/**
 * @brief Transformation de coordonnées pour une orientation est.
 *
 * @param dx Position locale X.
 * @param dy Position locale Y.
 * @param wx Pointeur vers la coordonnée X dans le monde.
 * @param wy Pointeur vers la coordonnée Y dans le monde.
 */
static void transform_east(int dx, int dy, int *wx, int *wy)
{
    *wx = -dy;
    *wy = dx;
}

/**
 * @brief Transformation de coordonnées pour une orientation sud.
 *
 * @param dx Position locale X.
 * @param dy Position locale Y.
 * @param wx Pointeur vers la coordonnée X dans le monde.
 * @param wy Pointeur vers la coordonnée Y dans le monde.
 */
static void transform_south(int dx, int dy, int *wx, int *wy)
{
    *wx = -dx;
    *wy = -dy;
}

/**
 * @brief Transformation de coordonnées pour une orientation ouest.
 *
 * @param dx Position locale X.
 * @param dy Position locale Y.
 * @param wx Pointeur vers la coordonnée X dans le monde.
 * @param wy Pointeur vers la coordonnée Y dans le monde.
 */
static void transform_west(int dx, int dy, int *wx, int *wy)
{
    *wx = dy;
    *wy = -dx;
}

/**
 * @brief Applique une transformation de coordonnées locales vers le monde
 * en fonction de l'orientation du joueur.
 *
 * @param transform La structure contenant la position locale et l’orientation.
 * @param world_x Pointeur vers la coordonnée X transformée dans le monde.
 * @param world_y Pointeur vers la coordonnée Y transformée dans le monde.
 */
void vision_transform_coordinates(vision_transform_t *transform,
    int *world_x, int *world_y)
{
    switch (transform->orientation) {
        case NORTH:
            transform_north(transform->local_x, transform->local_y,
                world_x, world_y);
            break;
        case EAST:
            transform_east(transform->local_x, transform->local_y,
                world_x, world_y);
            break;
        case SOUTH:
            transform_south(transform->local_x, transform->local_y,
                world_x, world_y);
            break;
        case WEST:
            transform_west(transform->local_x, transform->local_y,
                world_x, world_y);
            break;
    }
}
