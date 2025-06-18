/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_events
*/

#include "server.h"

static client_t *find_client_by_fd(server_t *server, int fd)
{
    client_t *client = server->clients;

    while (client) {
        if (client->fd == fd)
            return client;
        client = client->next;
    }
    return NULL;
}

static void handle_client_error(server_t *server, int i)
{
    client_t *client = find_client_by_fd(server, server->poll_fds[i].fd);

    if (client)
        server_disconnect_client(server, client);
}

static void handle_client_input(server_t *server, int i)
{
    client_t *client = find_client_by_fd(server, server->poll_fds[i].fd);
    int ret = 0;

    if (!client)
        return;
    ret = network_receive(server, client);
    if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        server_disconnect_client(server, client);
    else if (ret == 0)
        server_disconnect_client(server, client);
}

static void handle_single_client_event(server_t *server, int i)
{
    if (server->poll_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
        handle_client_error(server, i);
    else if (server->poll_fds[i].revents & POLLIN)
        handle_client_input(server, i);
}

static void handle_client_events(server_t *server)
{
    for (int i = 1; i < server->poll_count; i++)
        handle_single_client_event(server, i);
}

int handle_poll_events(server_t *server, int ret)
{
    if (server->poll_fds[0].revents & POLLIN)
        server_accept_client(server);
    if (ret > 0)
        handle_client_events(server);
    return 0;
}
