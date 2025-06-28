/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_formatter
*/

#include "vision.h"

static const char *RESOURCE_NAMES[] = {
    "food", "linemate", "deraumere", "sibur",
    "mendiane", "phiras", "thystame"
};

static void add_single_player(char *buffer, bool *first)
{
    if (!*first)
        strcat(buffer, " ");
    strcat(buffer, "player");
    *first = false;
}

static void add_players_to_content(server_t *server, int x, int y, char *buffer, bool *first)
{
    client_t *current = server->clients;
    int player_count = 0;

    while (current) {
        if (current->type == CLIENT_PLAYER && current->player &&
            current->player->x == x && current->player->y == y) {
            player_count++;
        }
        current = current->next;
    }
    for (int i = 0; i < player_count; i++) {
        add_single_player(buffer, first);
    }
}

static void add_resources_to_content(tile_t *tile, char *buffer, bool *first)
{
    for (int i = 0; i < NB_RESOURCES; i++) {
        for (int count = 0; count < tile->resources[i]; count++) {
            if (!*first)
                strcat(buffer, " ");
            strcat(buffer, RESOURCE_NAMES[i]);
            *first = false;
        }
    }
}

static void transform_tile_coordinates(server_t *server, player_t *player, 
    int local_x, int local_y, int *world_x, int *world_y)
{
    int dx = 0;
    int dy = 0;

    vision_apply_orientation(player->orientation, local_x, &dx);
    vision_apply_orientation(player->orientation, local_y, &dy);
    *world_x = (player->x + dx + server->game->width) % server->game->width;
    *world_y = (player->y + dy + server->game->height) % server->game->height;
}

void vision_format_tile_content(server_t *server, int x, int y, char *buffer)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    bool first = true;

    buffer[0] = '\0';
    if (!tile)
        return;
    add_players_to_content(server, x, y, buffer, &first);
    add_resources_to_content(tile, buffer, &first);
}

void vision_format_response(server_t *server, player_t *player, vision_data_t *data, char *buffer)
{
    char tile_content[512];
    bool first = true;
    int world_x = 0;
    int world_y = 0;

    strcpy(buffer, "[");
    for (int i = 0; i < data->tile_count; i++) {
        if (!first)
            strcat(buffer, ",");
        transform_tile_coordinates(server, player, data->tiles[i].x, 
            data->tiles[i].y, &world_x, &world_y);
        vision_format_tile_content(server, world_x, world_y, tile_content);
        strcat(buffer, tile_content);
        first = false;
    }
    strcat(buffer, "]");
}
