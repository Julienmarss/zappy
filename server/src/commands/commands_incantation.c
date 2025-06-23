/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_incantation
*/

#include "server.h"

static const elevation_req_t ELEVATION_REQUIREMENTS[] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 0, 0, 0},
    {3, 2, 2, 0, 1, 0, 2, 0},
    {4, 4, 1, 1, 2, 0, 1, 0},
    {5, 4, 1, 2, 1, 3, 0, 0},
    {6, 6, 1, 2, 3, 0, 1, 0},
    {7, 6, 2, 2, 2, 2, 2, 1}
};

static bool check_max_level(int level)
{
    return level >= 8;
}

static int count_same_level_players(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int count = 0;

    if (!tile)
        return 0;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i]->level == player->level)
            count++;
    }
    return count;
}

static bool check_player_requirements(server_t *server, player_t *player)
{
    elevation_req_t req = ELEVATION_REQUIREMENTS[player->level - 1];
    int same_level_count = count_same_level_players(server, player);

    if (same_level_count < req.nb_players)
        return false;
    return true;
}

static bool check_resource_requirements(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    elevation_req_t req = ELEVATION_REQUIREMENTS[player->level - 1];

    if (!tile)
        return false;
    if (tile->resources[LINEMATE] < req.linemate)
        return false;
    if (tile->resources[DERAUMERE] < req.deraumere)
        return false;
    if (tile->resources[SIBUR] < req.sibur)
        return false;
    if (tile->resources[MENDIANE] < req.mendiane)
        return false;
    if (tile->resources[PHIRAS] < req.phiras)
        return false;
    if (tile->resources[THYSTAME] < req.thystame)
        return false;
    return true;
}

static void consume_elevation_resources(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    elevation_req_t req = ELEVATION_REQUIREMENTS[player->level - 1];

    if (!tile)
        return;
    tile->resources[LINEMATE] -= req.linemate;
    tile->resources[DERAUMERE] -= req.deraumere;
    tile->resources[SIBUR] -= req.sibur;
    tile->resources[MENDIANE] -= req.mendiane;
    tile->resources[PHIRAS] -= req.phiras;
    tile->resources[THYSTAME] -= req.thystame;
}

static void elevate_single_player(player_t *player)
{
    char message[64];

    player->level++;
    snprintf(message, sizeof(message),
        "Elevation underway\nCurrent level: %d\n", player->level);
    if (player->client)
        network_send(player->client, message);
}

static void elevate_same_level_players(server_t *server, player_t *initiator)
{
    tile_t *tile = game_get_tile(server->game, initiator->x, initiator->y);

    if (!tile)
        return;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i]->level == initiator->level)
            elevate_single_player(tile->players[i]);
    }
}

static bool validate_incantation_request(client_t *client)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (check_max_level(client->player->level)) {
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

static bool check_incantation_requirements(server_t *server, player_t *player)
{
    if (!check_player_requirements(server, player)) {
        network_send(player->client, "ko\n");
        return false;
    }
    if (!check_resource_requirements(server, player)) {
        network_send(player->client, "ko\n");
        return false;
    }
    return true;
}

void cmd_incantation(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;

    (void)args;
    if (!validate_incantation_request(client))
        return;
    if (!check_incantation_requirements(server, player))
        return;
    consume_elevation_resources(server, player);
    elevate_same_level_players(server, player);
    gui_broadcast_player_action(server, player, "incantation");
}
