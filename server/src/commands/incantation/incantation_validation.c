/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation validation functions (2 public + 2 static)
*/

#include "incantation.h"

static bool is_player_at_max_level(int level)
{
    return level >= MAX_LEVEL;
}

static void send_validation_error(client_t *client)
{
    network_send(client, "ko\n");
}

bool validate_incantation_player(client_t *client)
{
    if (!client->player) {
        send_validation_error(client);
        return false;
    }
    if (is_player_at_max_level(client->player->level)) {
        send_validation_error(client);
        return false;
    }
    return true;
}

bool check_incantation_conditions(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    ctx->level = player->level;
    ctx->x = player->x;
    ctx->y = player->y;
    if (!check_player_requirements(server, player, ctx)) {
        send_validation_error(player->client);
        return false;
    }
    if (!check_resource_requirements(server, player)) {
        send_validation_error(player->client);
        return false;
    }
    return true;
}
