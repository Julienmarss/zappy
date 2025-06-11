/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Action commands implementation
*/

#include "server.h"

void cmd_fork(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ok\n");
}

void cmd_eject(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ok\n");
}

void cmd_take(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}

void cmd_set(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}

void cmd_incantation(t_server *server, t_client *client, char **args)
{
    (void)server;
    (void)args;
    network_send(client, "ko\n");
}