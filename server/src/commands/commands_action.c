/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Action commands implementation
*/

#include "server.h"

void cmd_fork(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ok\n");
}

void cmd_eject(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ok\n");
}

void cmd_take(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}

void cmd_set(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}

void cmd_incantation(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}
