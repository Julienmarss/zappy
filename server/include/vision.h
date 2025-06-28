/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision
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

// vision_builder.c
vision_data_t *vision_create_data(int level);
void vision_destroy_data(vision_data_t *data);

// vision_pattern.c
void vision_calculate_tiles(vision_data_t *data);

// vision_coordinates.c
void vision_apply_orientation(int orientation, int dx, int *world_dx);

// vision_formatter.c
void vision_format_response(server_t *server, player_t *player, vision_data_t *data, char *buffer);
void vision_format_tile_content(server_t *server, int x, int y, char *buffer);

#endif /* !VISION_H_ */
