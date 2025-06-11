/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server main loop
*/

#include "server.h"

static void handle_client_error(t_server *server, int i)
{
    t_client *client = server->clients;

    while (client) {
        if (client->fd == server->poll_fds[i].fd) {
            server_disconnect_client(server, client);
            break;
        }
        client = client->next;
    }
}

static void handle_client_input(t_server *server, int i)
{
    t_client *client = server->clients;

    while (client) {
        if (client->fd == server->poll_fds[i].fd) {
            if (network_receive(server, client) <= 0)
                server_disconnect_client(server, client);
            break;
        }
        client = client->next;
    }
}

static void handle_client_events(t_server *server)
{
    for (int i = 1; i < server->poll_count; i++) {
        if (server->poll_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
            handle_client_error(server, i);
            i--;
        } else if (server->poll_fds[i].revents & POLLIN) {
            handle_client_input(server, i);
            i--;
        }
    }
}

static int calculate_timeout(t_server *server)
{
    long current = get_time_microseconds();
    long elapsed = current - server->current_time;
    long time_unit_us = 1000000 / server->game->time_unit;
    int timeout = (time_unit_us - elapsed) / 1000;

    return (timeout < 0) ? 0 : timeout;
}

int server_run(t_server *server)
{
    int ret = 0;
    int timeout = 0;
    long current = 0;
    long time_unit_us = 1000000 / server->game->time_unit;

    while (1) {
        timeout = calculate_timeout(server);
        ret = poll(server->poll_fds, server->poll_count, timeout);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            perror("poll");
            return 84;
        }
        if (server->poll_fds[0].revents & POLLIN)
            server_accept_client(server);
        if (ret > 0)
            handle_client_events(server);
        current = get_time_microseconds();
        if (current - server->current_time >= time_unit_us) {
            server->current_time = current;
            game_update(server);
        }
    }
    return 0;
}
