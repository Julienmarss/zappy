/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_tile_content
*/

#include "vision.h"

static const char *RESOURCE_NAMES[] = {
    "food", "linemate", "deraumere", "sibur",
    "mendiane", "phiras", "thystame"
};

static void add_player_to_buffer(content_context_t *ctx)
{
    if (!*ctx->first)
        strcat(ctx->buffer, " ");
    strcat(ctx->buffer, "player");
    *ctx->first = false;
}

static void add_players_to_content(server_t *server, int x, int y,
    content_context_t *ctx)
{
    int player_count = vision_count_players_at_position(server, x, y);

    for (int i = 0; i < player_count; i++)
        add_player_to_buffer(ctx);
}

static void add_resource_to_buffer(content_context_t *ctx, const char *name)
{
    if (!*ctx->first)
        strcat(ctx->buffer, " ");
    strcat(ctx->buffer, name);
    *ctx->first = false;
}

static void add_resources_to_content(tile_t *tile, content_context_t *ctx)
{
    for (int i = 0; i < NB_RESOURCES; i++) {
        for (int count = 0; count < tile->resources[i]; count++)
            add_resource_to_buffer(ctx, RESOURCE_NAMES[i]);
    }
}

void vision_format_tile_content(server_t *server, int x, int y, char *buffer)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    bool first = true;
    content_context_t ctx = {buffer, &first};

    buffer[0] = '\0';
    if (!tile)
        return;
    add_players_to_content(server, x, y, &ctx);
    add_resources_to_content(tile, &ctx);
}
