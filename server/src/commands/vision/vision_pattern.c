/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_pattern
*/

#include "vision.h"

static void add_tile(vision_data_t *data, int *index, int x, int y)
{
    if (*index >= data->tile_count)
        return;
    data->tiles[*index].x = x;
    data->tiles[*index].y = y;
    data->tiles[*index].index = *index;
    (*index)++;
}

static void add_row(vision_data_t *data, int *index, int row)
{
    for (int col = -row; col <= row; col++)
        add_tile(data, index, col, -row);
}

static void build_pattern(vision_data_t *data)
{
    int index = 0;

    add_tile(data, &index, 0, 0);
    for (int row = 1; row <= data->level; row++)
        add_row(data, &index, row);
}

void vision_calculate_tiles(vision_data_t *data)
{
    if (!data)
        return;
    build_pattern(data);
}
