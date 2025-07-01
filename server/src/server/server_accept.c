/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_accept
*/

/**
 * @file server_accept.c
 * @brief Gère l'acceptation de nouveaux clients, leur ajout au poll et la déconnexion.
 */

#include "server.h"
#include <fcntl.h>

/**
 * @brief Crée une structure de client pour un nouveau client connecté.
 *
 * @param client_fd Descripteur de fichier du client.
 * @return Un pointeur vers le client nouvellement créé, ou NULL en cas d'erreur.
 */
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

/**
 * @brief Ajoute un client au tableau de poll.
 *
 * @param server Pointeur vers le serveur.
 * @param client_fd Descripteur du nouveau client.
 * @return true si l'ajout a réussi, false sinon.
 */
static bool add_client_to_poll(server_t *server, int client_fd)
{
    if (server->poll_count >= MAX_CLIENTS)
        return false;
    server->poll_fds[server->poll_count].fd = client_fd;
    server->poll_fds[server->poll_count].events = POLLIN;
    server->poll_count++;
    return true;
}

/**
 * @brief Ajoute un client à la liste chaînée des clients du serveur.
 *
 * @param server Pointeur vers le serveur.
 * @param client Client à ajouter.
 */
static void add_client_to_list(server_t *server, client_t *client)
{
    client->next = server->clients;
    server->clients = client;
}

/**
 * @brief Traite l’acceptation d’un client : création, ajout au poll et envoi du message WELCOME.
 *
 * @param server Pointeur vers le serveur.
 * @param client_fd Descripteur du client.
 * @param client_addr Adresse du client.
 */
static void handle_client_accept(server_t *server, int client_fd,
    struct sockaddr_in *client_addr)
{
    client_t *client = create_new_client(client_fd);

    if (!client)
        return;
    add_client_to_list(server, client);
    if (!add_client_to_poll(server, client_fd)) {
        server_disconnect_client(server, client);
        return;
    }
    network_send(client, "WELCOME\n");
    printf("New client connected from %s:%d\n",
        inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
}

/**
 * @brief Accepte un client si une connexion est en attente.
 *
 * @param server Pointeur vers le serveur.
 */
void server_accept_client(server_t *server)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = 0;

    client_fd = accept(server->socket_fd, (struct sockaddr *)&client_addr,
        &client_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept");
        return;
    }
    handle_client_accept(server, client_fd, &client_addr);
}

/**
 * @brief Retire un client du tableau de poll.
 *
 * @param server Pointeur vers le serveur.
 * @param fd Descripteur du client à retirer.
 */
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

/**
 * @brief Nettoie la file de commandes d’un client.
 *
 * @param client Pointeur vers le client.
 */
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

/**
 * @brief Retire un client de la liste chaînée du serveur.
 *
 * @param server Pointeur vers le serveur.
 * @param client Client à retirer.
 */
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

/**
 * @brief Déconnecte proprement un client du serveur.
 *
 * @param server Pointeur vers le serveur.
 * @param client Client à déconnecter.
 */
void server_disconnect_client(server_t *server, client_t *client)
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
