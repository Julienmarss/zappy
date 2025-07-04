/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

/**
 * @file server.c
 * @brief Contient les fonctions d'initialisation, création et destruction du serveur Zappy.
 */

#include "server.h"
#include <fcntl.h>

/**
 * @brief Crée une socket TCP.
 * 
 * @return Descripteur de socket en cas de succès, -1 en cas d'erreur.
 */
static int create_socket(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (sock < 0) {
        perror("socket");
        return -1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sock);
        return -1;
    }
    return sock;
}

/**
 * @brief Lie la socket à un port et l'écoute.
 * 
 * @param sock Descripteur de la socket.
 * @param port Port d'écoute.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
static int bind_and_listen(int sock, int port)
{
    struct sockaddr_in addr = {0};
    int flags = 0;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(sock, 128) < 0) {
        perror("listen");
        return -1;
    }
    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    return 0;
}

/**
 * @brief Configure la socket du serveur à partir d'un port donné.
 *
 * @param port Port d'écoute.
 * @return Descripteur de socket en cas de succès, -1 sinon.
 */
static int setup_server_socket(int port)
{
    int sock = create_socket();

    if (sock < 0)
        return -1;
    if (bind_and_listen(sock, port) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

/**
 * @brief Alloue une structure `server_t`.
 *
 * @return Pointeur vers la structure serveur, ou NULL en cas d’échec.
 */
static server_t *allocate_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));

    if (!server)
        return NULL;
    server->poll_fds = calloc(MAX_CLIENTS + 1, sizeof(struct pollfd));
    if (!server->poll_fds) {
        free(server);
        return NULL;
    }
    return server;
}

/**
 * @brief Initialise la socket du serveur et configure le `pollfd` initial.
 * 
 * @param server Structure serveur à configurer.
 * @param port Port d'écoute.
 * @return true en cas de succès, false sinon.
 */
static bool init_server_socket(server_t *server, int port)
{
    server->port = port;
    server->socket_fd = setup_server_socket(port);
    if (server->socket_fd < 0)
        return false;
    server->poll_fds[0].fd = server->socket_fd;
    server->poll_fds[0].events = POLLIN;
    server->poll_count = 1;
    return true;
}

/**
 * @brief Crée et initialise un nouveau serveur.
 * 
 * @param config Configuration initiale du serveur (taille, équipes, fréquence, etc.).
 * @return Pointeur vers `server_t` en cas de succès, NULL sinon.
 */
server_t *server_create(server_config_t *config)
{
    server_t *server = allocate_server();
    game_params_t params = {config->width, config->height, config->teams,
        config->nb_clients, config->freq};

    if (!server)
        return NULL;
    if (!init_server_socket(server, config->port)) {
        free(server->poll_fds);
        free(server);
        return NULL;
    }
    server->game = game_create(&params);
    if (!server->game) {
        close(server->socket_fd);
        free(server->poll_fds);
        free(server);
        return NULL;
    }
    server->current_time = get_time_microseconds();
    return server;
}

/**
 * @brief Libère toutes les ressources du serveur proprement.
 * 
 * @param server Pointeur vers le serveur à détruire.
 */
void server_destroy(server_t *server)
{
    client_t *next = NULL;

    if (!server)
        return;
    while (server->clients) {
        next = server->clients->next;
        server_disconnect_client(server, server->clients);
        server->clients = next;
    }
    if (server->socket_fd >= 0)
        close(server->socket_fd);
    free(server->poll_fds);
    game_destroy(server->game);
    free(server);
}
