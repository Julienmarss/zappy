/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_pattern
*/

#include "vision.h"

static void add_pattern_row(vision_data_t *data, int *index, int row)
{
    for (int col = -row; col <= row; col++) {
        if (*index >= data->tile_count)
            return;
        data->tiles[*index].x = col;
        data->tiles[*index].y = -row;
        data->tiles[*index].index = *index;
        (*index)++;
    }
}

static void build_vision_pattern(vision_data_t *data)
{
    int index = 0;
    int level = data->level;

    data->tiles[0].x = 0;
    data->tiles[0].y = 0;
    data->tiles[0].index = 0;
    index = 1;
    for (int row = 1; row <= level; row++) {
        add_pattern_row(data, &index, row);
    }
}

void vision_calculate_tiles(vision_data_t *data)
{
    build_vision_pattern(data);
}
