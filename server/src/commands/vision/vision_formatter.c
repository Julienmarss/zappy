/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_formatter
*/

#include "vision.h"

static void format_single_tile(vision_format_t *format, int tile_index,
    char *buffer, bool *first)
{
    char tile_content[512];
    int world_x = 0;
    int world_y = 0;

    if (!*first)
        strcat(buffer, ",");
    vision_get_world_coordinates(format, tile_index, &world_x, &world_y);
    vision_format_tile_content(format->server, world_x, world_y,
        tile_content);
    strcat(buffer, tile_content);
    *first = false;
}

static void add_opening_bracket(char *buffer)
{
    strcpy(buffer, "[");
}

static void add_closing_bracket(char *buffer)
{
    strcat(buffer, "]");
}

static void process_all_tiles(vision_format_t *format, char *buffer)
{
    bool first = true;

    for (int i = 0; i < format->data->tile_count; i++)
        format_single_tile(format, i, buffer, &first);
}

void vision_format_response(vision_format_t *format, char *buffer)
{
    add_opening_bracket(buffer);
    process_all_tiles(format, buffer);
    add_closing_bracket(buffer);
}
