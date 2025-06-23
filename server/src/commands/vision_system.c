/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_system
*/

#include "server.h"

static position_t rotate_coordinates(int dx, int dy, int orientation)
{
    position_t pos = {0};

    if (orientation == NORTH) {
        pos.x = dx;
        pos.y = dy;
        return pos;
    }
    if (orientation == EAST) {
        pos.x = -dy;
        pos.y = dx;
        return pos;
    }
    if (orientation == SOUTH) {
        pos.x = -dx;
        pos.y = -dy;
        return pos;
    }
    pos.x = dy;
    pos.y = -dx;
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

static void add_vision_tile(vision_params_t *params, int dx, int dy)
{
    position_t world_pos = get_world_position(params->server, params->player,
        dx, dy);
    tile_t *tile = NULL;
    bool is_current = false;

    tile = game_get_tile(params->server->game, world_pos.x, world_pos.y);
    is_current = (dx == 0 && dy == 0);
    if (!params->is_first_tile)
        strcat(params->buffer, ",");
    build_tile_content(params->buffer, tile, is_current);
    params->is_first_tile = false;
}

static void build_level_one_vision(server_t *server, player_t *player,
    char *buffer)
{
    vision_params_t params = {server, player, buffer, true};

    add_vision_tile(&params, 0, 0);
    add_vision_tile(&params, -1, -1);
    add_vision_tile(&params, 0, -1);
    add_vision_tile(&params, 1, -1);
}

void build_player_vision(server_t *server, player_t *player, char *buffer)
{
    if (player->level == 1) {
        build_level_one_vision(server, player, buffer);
        return;
    }
    build_level_one_vision(server, player, buffer);
}
