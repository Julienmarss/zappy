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

void cmd_look(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char buffer[4096] = "[";
    tile_t *tile = NULL;
    bool first = true;

    (void)args;
    if (!player) {
        printf("DEBUG: Look - No player\n");
        network_send(client, "ko\n");
        return;
    }
    printf("DEBUG: Look - Player ID %d at (%d,%d), level %d\n", 
           player->id, player->x, player->y, player->level);
    tile = game_get_tile(server->game, player->x, player->y);
    if (!tile) {
        printf("DEBUG: Look - Could not get tile\n");
        network_send(client, "[]\n");
        return;
    }
    strcat(buffer, "player");
    first = false;
    
    for (int i = 0; i < NB_RESOURCES; i++) {
        if (tile->resources[i] > 0) {
            for (int count = 0; count < tile->resources[i]; count++) {
                if (!first)
                    strcat(buffer, " ");
                strcat(buffer, RESOURCE_NAMES[i]);
                first = false;
            }
        }
    }
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
