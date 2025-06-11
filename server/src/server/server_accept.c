/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server client acceptance
*/

#include "server.h"
#include <fcntl.h>

static client_t *create_new_client(int client_fd)
{
    client_t *client = calloc(1, sizeof(client_t));
    int flags = 0;

    if (!client) {
        close(client_fd);
        return NULL;
    }
    flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    client->fd = client_fd;
    client->type = CLIENT_UNKNOWN;
    return client;
}

static bool add_client_to_poll(server_t *server, int client_fd)
{
    if (server->poll_count >= MAX_CLIENTS)
        return false;
    server->poll_fds[server->poll_count].fd = client_fd;
    server->poll_fds[server->poll_count].events = POLLIN;
    server->poll_count++;
    return true;
}

static void add_client_to_list(server_t *server, client_t *client)
{
    client->next = server->clients;
    server->clients = client;
}

void server_accepclient_t(server_t *server)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = 0;
    client_t *client = NULL;

    client_fd = accept(server->socket_fd, (struct sockaddr *)&client_addr,
        &client_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept");
        return;
    }
    client = create_new_client(client_fd);
    if (!client)
        return;
    add_client_to_list(server, client);
    if (!add_client_to_poll(server, client_fd)) {
        server_disconnect_client(server, client);
        return;
    }
    network_send(client, "WELCOME\n");
    printf("New client connected from %s:%d\n",
        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

static void remove_from_poll(server_t *server, int fd)
{
    for (int i = 1; i < server->poll_count; i++) {
        if (server->poll_fds[i].fd == fd) {
            memmove(&server->poll_fds[i], &server->poll_fds[i + 1],
                (server->poll_count - i - 1) * sizeof(struct pollfd));
            server->poll_count--;
            break;
        }
    }
}

static void cleanup_command_queue(client_t *client)
{
    command_t *next = NULL;

    while (client->cmd_queue) {
        next = client->cmd_queue->next;
        free(client->cmd_queue->cmd);
        free(client->cmd_queue);
        client->cmd_queue = next;
    }
}

static void remove_client_from_list(server_t *server, client_t *client)
{
    client_t **current = &server->clients;

    while (*current) {
        if (*current == client) {
            *current = client->next;
            break;
        }
        current = &(*current)->next;
    }
}

void server_disconnecclient(server_t *server, client_t *client)
{
    if (!client)
        return;
    printf("Client disconnected (fd: %d)\n", client->fd);
    remove_from_poll(server, client->fd);
    if (client->player)
        player_die(server, client->player);
    remove_client_from_list(server, client);
    cleanup_command_queue(client);
    close(client->fd);
    free(client);
}
