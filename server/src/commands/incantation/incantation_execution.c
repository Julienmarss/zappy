/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation execution functions (1 public + 4 static)
*/

#include "incantation.h"
#include "gui_protocol.h"

static void consume_elevation_resources(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    const elevation_req_t *req = get_elevation_requirements(player->level);

    if (!tile || !req) {
        return;
    }
    tile->resources[LINEMATE] -= req->linemate;
    tile->resources[DERAUMERE] -= req->deraumere;
    tile->resources[SIBUR] -= req->sibur;
    tile->resources[MENDIANE] -= req->mendiane;
    tile->resources[PHIRAS] -= req->phiras;
    tile->resources[THYSTAME] -= req->thystame;
}

static void elevate_single_player(player_t *player)
{
    char message[64];

    player->level++;
    snprintf(message, sizeof(message),
        "Elevation underway\nCurrent level: %d\n", player->level);
    if (player->client) {
        network_send(player->client, message);
    }
}

static void elevate_participants(incantation_ctx_t *ctx)
{
    for (int i = 0; i < ctx->nb_participants; i++) {
        if (ctx->participants[i]) {
            elevate_single_player(ctx->participants[i]);
        }
    }
}

static void broadcast_player_updates(server_t *server, incantation_ctx_t *ctx)
{
    for (int i = 0; i < ctx->nb_participants; i++) {
        if (ctx->participants[i]) {
            gui_broadcast_player_level(server, ctx->participants[i]);
            gui_broadcast_player_inventory(server, ctx->participants[i]);
        }
    }
}

void execute_incantation_ritual(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    gui_broadcast_incantation_start(server, ctx->x, ctx->y, ctx->level,
        ctx->participants, ctx->nb_participants);
    consume_elevation_resources(server, player);
    elevate_participants(ctx);
    gui_broadcast_incantation_end(server, ctx->x, ctx->y, true);
    broadcast_player_updates(server, ctx);
    check_game_end_condition(server);
    if (ctx->participants) {
        free(ctx->participants);
    }
}
