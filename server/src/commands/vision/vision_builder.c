/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_builder
*/

#include "vision.h"

static int calculate_tile_count(int level)
{
    int total = 0;

    if (level < 1 || level > 8)
        level = 1;
    for (int row = 0; row <= level; row++)
        total += (2 * row + 1);
    return total;
}

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

static void init_vision_data(vision_data_t *data, int level)
{
    if (level < 1 || level > 8)
        level = 1;
    data->tile_count = calculate_tile_count(level);
    data->level = level;
}

vision_data_t *vision_create_data(int level)
{
    vision_data_t *data = allocate_vision_data(level);

    if (!data)
        return NULL;
    init_vision_data(data, level);
    return data;
}

void vision_destroy_data(vision_data_t *data)
{
    if (!data)
        return;
    if (data->tiles)
        free(data->tiles);
    free(data);
}
