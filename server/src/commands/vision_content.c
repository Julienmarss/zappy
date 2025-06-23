/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_content
*/

#include "server.h"

static const char *RESOURCE_NAMES[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

static bool add_single_resource_safe(char *buffer, int resource_index,
    bool *first)
{
    int needed_space = 0;

    if (!*first)
        needed_space = strlen(RESOURCE_NAMES[resource_index]) + 1;
    else
        needed_space = strlen(RESOURCE_NAMES[resource_index]);
    if (strlen(buffer) + needed_space >= MAX_BUFFER_SIZE - 10)
        return false;
    if (!*first)
        strcat(buffer, " ");
    strcat(buffer, RESOURCE_NAMES[resource_index]);
    *first = false;
    return true;
}

static bool add_single_player_safe(char *buffer, bool *first)
{
    int needed_space = *first ? 6 : 7;

    if (strlen(buffer) + needed_space >= MAX_BUFFER_SIZE - 10)
        return false;
    if (!*first)
        strcat(buffer, " ");
    strcat(buffer, "player");
    *first = false;
    return true;
}

static void add_single_resource_type(char *buffer, int resource_index,
    int count, bool *first)
{
    for (int j = 0; j < count; j++) {
        if (!add_single_resource_safe(buffer, resource_index, first))
            return;
    }
}

static void add_tile_resources_safe(char *buffer, tile_t *tile, bool *first)
{
    int added = 0;
    int count = 0;

    for (int i = 0; i < NB_RESOURCES && added < 10; i++) {
        count = tile->resources[i];
        if (count > 10 - added)
            count = 10 - added;
        add_single_resource_type(buffer, i, count, first);
        added += count;
    }
}

static void add_other_players_safe(char *buffer, tile_t *tile, bool *first,
    bool is_current_tile)
{
    int player_count = 0;
    int i = 0;

    player_count = is_current_tile ? tile->player_count - 1 :
        tile->player_count;
    for (i = 0; i < player_count && i < 5; i++) {
        if (!add_single_player_safe(buffer, first))
            break;
    }
}

void build_tile_content(char *buffer, tile_t *tile, bool is_current_tile)
{
    bool first = true;

    if (!tile) {
        strcat(buffer, ",");
        return;
    }
    if (is_current_tile)
        add_single_player_safe(buffer, &first);
    add_other_players_safe(buffer, tile, &first, is_current_tile);
    add_tile_resources_safe(buffer, tile, &first);
}
