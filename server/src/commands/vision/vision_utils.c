/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_utils
*/

#include "vision.h"

static bool is_player_at_position(client_t *client, int x, int y)
{
    return (client->type == CLIENT_PLAYER && client->player &&
        client->player->x == x && client->player->y == y);
}

static void calculate_world_position(vision_format_t *format, int tile_index,
    int *world_x, int *world_y)
{
    vision_transform_t transform = {
        .orientation = format->player->orientation,
        .local_x = format->data->tiles[tile_index].x,
        .local_y = format->data->tiles[tile_index].y
    };
    int dx = 0;
    int dy = 0;

    vision_transform_coordinates(&transform, &dx, &dy);
    *world_x = (format->player->x + dx + format->server->game->width)
        % format->server->game->width;
    *world_y = (format->player->y + dy + format->server->game->height)
        % format->server->game->height;
}

int vision_count_players_at_position(server_t *server, int x, int y)
{
    client_t *current = server->clients;
    int count = 0;

    while (current) {
        if (is_player_at_position(current, x, y))
            count++;
        current = current->next;
    }
    return count;
}

void vision_get_world_coordinates(vision_format_t *format, int tile_index,
    int *world_x, int *world_y)
{
    calculate_world_position(format, tile_index, world_x, world_y);
}
