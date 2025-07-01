/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_events
*/

/**
 * @file server_events.c
 * @brief Gère les événements de clients (connexion, lecture, erreur) via poll.
 */

#include "server.h"

/**
 * @brief Trouve un client à partir de son descripteur de fichier.
 *
 * @param server Pointeur vers la structure serveur.
 * @param fd Descripteur de fichier du client.
 * @return Pointeur vers le client trouvé ou NULL s'il n'existe pas.
 */
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

/**
 * @brief Gère une erreur détectée sur un client (déconnexion).
 *
 * @param server Pointeur vers le serveur.
 * @param i Index du client dans le tableau poll_fds.
 */
static void handle_client_error(server_t *server, int i)
{
    client_t *client = find_client_by_fd(server, server->poll_fds[i].fd);

    if (client)
        server_disconnect_client(server, client);
}

/**
 * @brief Gère la lecture d'un client s'il a envoyé des données.
 *
 * @param server Pointeur vers le serveur.
 * @param i Index du client dans le tableau poll_fds.
 */
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

/**
 * @brief Gère les événements pour un client spécifique (entrée ou erreur).
 *
 * @param server Pointeur vers le serveur.
 * @param i Index du client dans le tableau poll_fds.
 */
static void handle_single_client_event(server_t *server, int i)
{
    if (server->poll_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
        handle_client_error(server, i);
    else if (server->poll_fds[i].revents & POLLIN)
        handle_client_input(server, i);
}

/**
 * @brief Parcourt tous les clients et traite leurs événements s’ils en ont.
 *
 * @param server Pointeur vers le serveur.
 */
static void handle_client_events(server_t *server)
{
    for (int i = 1; i < server->poll_count; i++)
        handle_single_client_event(server, i);
}

/**
 * @brief Gère tous les événements détectés par poll().
 *
 * @param server Pointeur vers le serveur.
 * @param ret Valeur retournée par poll().
 * @return Toujours 0.
 */
int handle_poll_events(server_t *server, int ret)
{
    if (server->poll_fds[0].revents & POLLIN)
        server_accept_client(server);
    if (ret > 0)
        handle_client_events(server);
    return 0;
}
