/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Vision system core - coordinates and tile management
*/

#include "server.h"

static position_t rotate_coordinates(int dx, int dy, int orientation)
{
    position_t pos = {0};

    if (orientation == NORTH) {
        pos.x = dx;
        pos.y = dy;
    } else if (orientation == EAST) {
        pos.x = -dy;
        pos.y = dx;
    } else if (orientation == SOUTH) {
        pos.x = -dx;
        pos.y = -dy;
    } else if (orientation == WEST) {
        pos.x = dy;
        pos.y = -dx;
    }
    return pos;
}

static position_t get_world_position(server_t *server, player_t *player,
    int dx, int dy)
{
    position_t relative = rotate_coordinates(dx, dy, player->orientation);
    position_t world = {0};

    world.x = (player->x + relative.x + server->game->width) %
        server->game->width;
    world.y = (player->y + relative.y + server->game->height) %
        server->game->height;
    return world;
}

static void add_vision_tile(server_t *server, player_t *player,
    char *buffer, int dx, int dy, bool is_first_tile)
{
    position_t world_pos = get_world_position(server, player, dx, dy);
    tile_t *tile = NULL;
    bool is_current = false;

    tile = game_get_tile(server->game, world_pos.x, world_pos.y);
    is_current = (dx == 0 && dy == 0);
    if (!is_first_tile)
        strcat(buffer, ",");
    build_tile_content(buffer, tile, is_current);
}

static void build_level_one_vision(server_t *server, player_t *player,
    char *buffer)
{
    add_vision_tile(server, player, buffer, 0, 0, true);
    add_vision_tile(server, player, buffer, -1, -1, false);
    add_vision_tile(server, player, buffer, 0, -1, false);
    add_vision_tile(server, player, buffer, 1, -1, false);
}

void build_player_vision(server_t *server, player_t *player, char *buffer)
{
    if (player->level == 1) {
        build_level_one_vision(server, player, buffer);
        return;
    }
    build_level_one_vision(server, player, buffer);
}
