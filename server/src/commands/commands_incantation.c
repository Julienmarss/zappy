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

    if (same_level_count < req.nb_players) {
        printf("DEBUG: Not enough players of level %d (need %d, have %d)\n",
            player->level, req.nb_players, same_level_count);
        return false;
    }
    return true;
}

static bool check_resource_requirements(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    elevation_req_t req = ELEVATION_REQUIREMENTS[player->level - 1];

    if (!tile)
        return false;
    if (tile->resources[LINEMATE] < req.linemate ||
        tile->resources[DERAUMERE] < req.deraumere ||
        tile->resources[SIBUR] < req.sibur ||
        tile->resources[MENDIANE] < req.mendiane ||
        tile->resources[PHIRAS] < req.phiras ||
        tile->resources[THYSTAME] < req.thystame) {
        printf("DEBUG: Not enough resources on tile for level %d\n",
            player->level);
        return false;
    }
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
    printf("DEBUG: Consumed resources for elevation to level %d\n",
        player->level + 1);
}

static void elevate_same_level_players(server_t *server, player_t *initiator)
{
    tile_t *tile = game_get_tile(server->game, initiator->x, initiator->y);
    char message[64];
    int elevated_count = 0;

    if (!tile)
        return;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i]->level == initiator->level) {
            tile->players[i]->level++;
            snprintf(message, sizeof(message),
                "Elevation underway\nCurrent level: %d\n",
                tile->players[i]->level);
            if (tile->players[i]->client)
                network_send(tile->players[i]->client, message);
            elevated_count++;
            printf("DEBUG: Player %d elevated to level %d\n",
                tile->players[i]->id, tile->players[i]->level);
        }
    }
    printf("DEBUG: Elevated %d players from level %d to %d\n",
        elevated_count, initiator->level, initiator->level + 1);
}

static bool validate_incantation_request(client_t *client)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (check_max_level(client->player->level)) {
        printf("DEBUG: Player %d already at max level %d\n",
            client->player->id, client->player->level);
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
    printf("DEBUG: Player %d attempting incantation at level %d\n",
        player->id, player->level);
    if (!check_incantation_requirements(server, player))
        return;
    consume_elevation_resources(server, player);
    elevate_same_level_players(server, player);
    gui_broadcast_player_action(server, player, "incantation");
}
