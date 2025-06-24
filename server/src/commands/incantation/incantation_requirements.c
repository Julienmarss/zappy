/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation requirements checking functions (3 public + 5 static)
*/

#include "incantation.h"

static const elevation_req_t ELEVATION_REQUIREMENTS[] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 0, 0, 0},
    {3, 2, 2, 0, 1, 0, 2, 0},
    {4, 4, 1, 1, 2, 0, 1, 0},
    {5, 4, 1, 2, 1, 3, 0, 0},
    {6, 6, 1, 2, 3, 0, 1, 0},
    {7, 6, 2, 2, 2, 2, 2, 1}
};

static int count_same_level_players(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int count = 0;

    if (!tile) {
        return 0;
    }
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i]->level == player->level) {
            count++;
        }
    }
    return count;
}

static void collect_participants(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int count = 0;

    if (!tile) {
        return;
    }
    for (int i = 0; i < tile->player_count && count < MAX_PARTICIPANTS; i++) {
        if (tile->players[i]->level == player->level) {
            ctx->participants[count] = tile->players[i];
            count++;
        }
    }
    ctx->nb_participants = count;
}

static bool check_single_resource(tile_t *tile, int resource_type,
    int required_amount)
{
    return tile->resources[resource_type] >= required_amount;
}

static bool validate_tile_and_requirements(tile_t *tile,
    const elevation_req_t *req)
{
    return tile != NULL && req != NULL;
}

const elevation_req_t *get_elevation_requirements(int level)
{
    if (level < 1 || level > 7) {
        return NULL;
    }
    return &ELEVATION_REQUIREMENTS[level - 1];
}

bool check_player_requirements(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    const elevation_req_t *req = get_elevation_requirements(player->level);
    int same_level_count = count_same_level_players(server, player);

    if (!req) {
        return false;
    }
    if (same_level_count < req->nb_players) {
        return false;
    }
    ctx->participants = malloc(sizeof(player_t *) * MAX_PARTICIPANTS);
    if (!ctx->participants) {
        return false;
    }
    collect_participants(server, player, ctx);
    return true;
}

bool check_resource_requirements(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    const elevation_req_t *req = get_elevation_requirements(player->level);

    if (!validate_tile_and_requirements(tile, req)) {
        return false;
    }
    if (!check_single_resource(tile, LINEMATE, req->linemate)) {
        return false;
    }
    if (!check_single_resource(tile, DERAUMERE, req->deraumere)) {
        return false;
    }
    if (!check_single_resource(tile, SIBUR, req->sibur)) {
        return false;
    }
    if (!check_single_resource(tile, MENDIANE, req->mendiane)) {
        return false;
    }
    if (!check_single_resource(tile, PHIRAS, req->phiras)) {
        return false;
    }
    if (!check_single_resource(tile, THYSTAME, req->thystame)) {
        return false;
    }
    return true;
}
