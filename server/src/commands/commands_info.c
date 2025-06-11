/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Information commands implementation
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

void cmd_look(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "[player]\n");
}

void cmd_inventory(t_server *server, t_client *client, char **args)
{
    t_player *player = client->player;
    char buffer[512] = "[";
    char tmp[64];

    (void)server;
    (void)args;
    if (!player)
        return;
    for (int i = 0; i < NB_RESOURCES; i++) {
        snprintf(tmp, sizeof(tmp), "%s %d", 
            RESOURCE_NAMES[i], player->inventory[i]);
        strcat(buffer, tmp);
        if (i < NB_RESOURCES - 1)
            strcat(buffer, ", ");
    }
    strcat(buffer, "]\n");
    network_send(client, buffer);
}

void cmd_broadcast(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ok\n");
}

void cmd_connect_nbr(t_server *server, t_client *client, char **args)
{
    t_player *player = client->player;
    t_team *team = NULL;
    char buffer[32];

    (void)server;
    (void)args;
    if (!player || !player->team)
        return;
    team = player->team;
    snprintf(buffer, sizeof(buffer), "%d\n",
        team->max_clients - team->connected_clients);
    network_send(client, buffer);
}