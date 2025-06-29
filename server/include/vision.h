/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision system header
*/

#ifndef VISION_H_
    #define VISION_H_

    #include "server.h"

typedef struct vision_tile_s {
    int x;
    int y;
    int index;
} vision_tile_t;

typedef struct vision_data_s {
    vision_tile_t *tiles;
    int tile_count;
    int level;
} vision_data_t;

typedef struct vision_transform_s {
    int orientation;
    int local_x;
    int local_y;
} vision_transform_t;

typedef struct vision_format_s {
    server_t *server;
    player_t *player;
    vision_data_t *data;
} vision_format_t;

typedef struct content_context_s {
    char *buffer;
    bool *first;
} content_context_t;

// vision_builder.c
vision_data_t *vision_create_data(int level);
void vision_destroy_data(vision_data_t *data);

// vision_pattern.c
void vision_calculate_tiles(vision_data_t *data);

// vision_coordinates.c
void vision_transform_coordinates(vision_transform_t *transform,
    int *world_x, int *world_y);

// vision_formatter.c
void vision_format_response(vision_format_t *format, char *buffer);

// vision_tile_content.c
void vision_format_tile_content(server_t *server, int x, int y,
    char *buffer);

// vision_utils.c
int vision_count_players_at_position(server_t *server, int x, int y);
void vision_get_world_coordinates(vision_format_t *format, int tile_index,
    int *world_x, int *world_y);

#endif /* !VISION_H_ */
