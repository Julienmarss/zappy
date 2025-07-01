/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_action
*/

/**
 * @file gui_broadcast_action.c
 * @brief Fonctions de broadcast vers les clients graphiques (expulsion, fork, message).
 */

#include "gui_protocol.h"

/**
 * @brief Vérifie si un joueur est valide (non NULL).
 *
 * @param player Le pointeur vers le joueur.
 * @return true si valide, false sinon.
 */
static bool is_player_valid(player_t *player)
{
    return player != NULL;
}

/**
 * @brief Vérifie si un message est valide (non NULL).
 *
 * @param message Le message à vérifier.
 * @return true si valide, false sinon.
 */
static bool is_message_valid(const char *message)
{
    return message != NULL;
}

/**
 * @brief Notifie les clients graphiques qu’un joueur a expulsé quelqu’un.
 *
 * Envoie le message `pex #<id>\n` à tous les clients graphiques.
 *
 * @param server Le serveur.
 * @param player Le joueur ayant expulsé.
 */
void gui_broadcast_player_expulsion(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pex #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Notifie les clients graphiques qu’un joueur a fork (pondu un œuf).
 *
 * Envoie le message `pfk #<id>\n` à tous les clients graphiques.
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 */
void gui_broadcast_player_fork(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pfk #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Notifie les clients graphiques qu’un joueur a envoyé un broadcast.
 *
 * Envoie le message `pbc #<id> <message>\n` à tous les clients graphiques.
 *
 * @param server Le serveur.
 * @param player Le joueur ayant broadcast.
 * @param broadcast_message Le message broadcasté.
 */
void gui_broadcast_player_broadcast(server_t *server, player_t *player,
    const char *broadcast_message)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player) || !is_message_valid(broadcast_message)) {
        return;
    }
    snprintf(message, sizeof(message), "pbc #%d %s\n",
        player->id, broadcast_message);
    gui_send_to_all_graphic_clients(server, message);
}
