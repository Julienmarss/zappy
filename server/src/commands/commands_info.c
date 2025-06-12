/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Information commands implementation - SIMPLE SAFE VERSION
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

static bool validate_player_for_look(client_t *client)
{
    if (!client->player) {
        printf("DEBUG: Look - No player\n");
        network_send(client, "ko\n");
        return false;
    }
    printf("DEBUG: Look - Player ID %d at (%d,%d), level %d\n",
        client->player->id, client->player->x, client->player->y,
        client->player->level);
    return true;
}

static void add_single_resource(char *buffer, int resource_type, bool *first)
{
    if (!*first)
        strcat(buffer, " ");
    strcat(buffer, RESOURCE_NAMES[resource_type]);
    *first = false;
}

static void add_resource_type_to_buffer(char *buffer, int resource_type,
    int quantity, bool *first)
{
    for (int count = 0; count < quantity; count++)
        add_single_resource(buffer, resource_type, first);
}

static void add_resources_to_buffer(char *buffer, tile_t *tile, bool *first)
{
    for (int i = 0; i < NB_RESOURCES; i++) {
        if (tile->resources[i] > 0)
            add_resource_type_to_buffer(buffer, i, tile->resources[i], first);
    }
}

void cmd_look(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char buffer[4096] = "[";
    tile_t *tile = NULL;
    bool first = true;

    (void)args;
    if (!validate_player_for_look(client))
        return;
    tile = game_get_tile(server->game, player->x, player->y);
    if (!tile) {
        printf("DEBUG: Look - Could not get tile\n");
        network_send(client, "[]\n");
        return;
    }
    strcat(buffer, "player");
    first = false;
    add_resources_to_buffer(buffer, tile, &first);
    strcat(buffer, "]\n");
    printf("DEBUG: Look result: %s", buffer);
    network_send(client, buffer);
}

void cmd_inventory(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char buffer[512] = "[";
    char tmp[64];

    (void)server;
    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    for (int i = 0; i < NB_RESOURCES; i++) {
        snprintf(tmp, sizeof(tmp), "%s %d",
            RESOURCE_NAMES[i], player->inventory[i]);
        strcat(buffer, tmp);
        if (i < NB_RESOURCES - 1)
            strcat(buffer, ", ");
    }
    strcat(buffer, "]\n");
    printf("DEBUG: Inventory: %s", buffer);
    network_send(client, buffer);
}

void cmd_broadcast(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    printf("DEBUG: Broadcast command (not implemented yet)\n");
    network_send(client, "ok\n");
}

void cmd_connect_nbr(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    team_t *team = NULL;
    char buffer[32];

    (void)server;
    (void)args;
    if (!player || !player->team) {
        network_send(client, "ko\n");
        return;
    }
    team = player->team;
    snprintf(buffer, sizeof(buffer), "%d\n",
        team->max_clients - team->connected_clients);
    printf("DEBUG: Connect_nbr: %s", buffer);
    network_send(client, buffer);
}
