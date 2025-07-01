/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_handler
*/

/**
 * @file network_handler.c
 * @brief Gestion de la réception des lignes envoyées par les clients (joueurs ou GUI).
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Crée une nouvelle commande à partir d'une ligne de texte.
 *
 * @param line Ligne reçue du client.
 * @return Pointeur vers une nouvelle commande, ou NULL en cas d'erreur.
 */
static command_t *create_command(const char *line)
{
    command_t *cmd = calloc(1, sizeof(command_t));

    if (!cmd)
        return NULL;
    cmd->cmd = strdup(line);
    if (!cmd->cmd) {
        free(cmd);
        return NULL;
    }
    str_trim(cmd->cmd);  // Nettoie la commande (supprime \n, \r, etc.)
    cmd->time_limit = 0;
    cmd->next = NULL;
    return cmd;
}

/**
 * @brief Ajoute une commande à la file d'attente du client.
 *
 * @param client Le client joueur.
 * @param cmd La commande à ajouter.
 */
static void add_command_to_queue(client_t *client, command_t *cmd)
{
    command_t *last = client->cmd_queue;

    if (!client->cmd_queue) {
        client->cmd_queue = cmd;
    } else {
        while (last->next)
            last = last->next;
        last->next = cmd;
    }
    client->cmd_count++;
}

/**
 * @brief Gère une ligne envoyée par un client inconnu (non encore authentifié).
 *
 * @param server Le serveur.
 * @param client Le client.
 * @param line La ligne reçue.
 */
static void handle_unknown_client(server_t *server, client_t *client,
    const char *line)
{
    network_handle_new_client(server, client, line);
}

/**
 * @brief Gère une ligne envoyée par un client joueur.
 *
 * @param client Le client joueur.
 * @param line La ligne reçue.
 */
static void handle_player_client(client_t *client, const char *line)
{
    command_t *cmd = NULL;

    if (client->cmd_count < MAX_CMD_QUEUE) {
        cmd = create_command(line);
        if (cmd)
            add_command_to_queue(client, cmd);
    }
}

/**
 * @brief Gère une ligne envoyée par un client graphique.
 *
 * @param server Le serveur.
 * @param client Le client graphique.
 * @param line La ligne reçue.
 */
static void handle_graphic_client(server_t *server, client_t *client,
    const char *line)
{
    printf("DEBUG: GUI client sent command: '%s'\n", line);
    gui_handle_command(server, client, line);
}

/**
 * @brief Point d'entrée unique pour traiter une ligne reçue d'un client.
 *
 * @param server Le serveur.
 * @param client Le client concerné.
 * @param line La ligne de commande reçue.
 */
void network_handle_client_line(server_t *server, client_t *client,
    const char *line)
{
    if (client->type == CLIENT_UNKNOWN) {
        handle_unknown_client(server, client, line);
        return;
    }
    if (client->type == CLIENT_PLAYER) {
        handle_player_client(client, line);
        return;
    }
    if (client->type == CLIENT_GRAPHIC) {
        handle_graphic_client(server, client, line);
        return;
    }
    printf("DEBUG: Unknown client type: %d\n", client->type);
}
