/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_coordinates
*/

#include "vision.h"

static void transform_north(int dx, int dy, int *wx, int *wy)
{
    *wx = dx;
    *wy = dy;
}

static void transform_east(int dx, int dy, int *wx, int *wy)
{
    *wx = -dy;
    *wy = dx;
}

static void transform_south(int dx, int dy, int *wx, int *wy)
{
    *wx = -dx;
    *wy = -dy;
}

static void transform_west(int dx, int dy, int *wx, int *wy)
{
    *wx = dy;
    *wy = -dx;
}

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
