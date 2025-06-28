/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_coordinates
*/

#include "vision.h"

static void apply_north_orientation(int dx, int dy, int *world_dx, int *world_dy)
{
    *world_dx = dx;
    *world_dy = dy;
}

static void apply_east_orientation(int dx, int dy, int *world_dx, int *world_dy)
{
    *world_dx = -dy;
    *world_dy = dx;
}

static void apply_south_orientation(int dx, int dy, int *world_dx, int *world_dy)
{
    *world_dx = -dx;
    *world_dy = -dy;
}

static void apply_west_orientation(int dx, int dy, int *world_dx, int *world_dy)
{
    *world_dx = dy;
    *world_dy = -dx;
}

static void apply_default_orientation(int dx, int dy, int *world_dx, int *world_dy)
{
    (void)dx;
    (void)dy;
    *world_dx = 0;
    *world_dy = 0;
}

void vision_apply_orientation(int orientation, int dx, int *world_dx)
{
    static void (*orientation_funcs[])(int, int, int*, int*) = {
        apply_default_orientation,
        apply_north_orientation,
        apply_east_orientation,
        apply_south_orientation,
        apply_west_orientation
    };
    int index = (orientation >= 1 && orientation <= 4) ? orientation : 0;
    int world_dy = 0;

    orientation_funcs[index](dx, 0, world_dx, &world_dy);
}
