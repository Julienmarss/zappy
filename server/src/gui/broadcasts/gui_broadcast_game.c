/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_game
*/

/**
 * @file gui_broadcast_game.c
 * @brief Contient les fonctions de broadcast d'événements globaux de la partie
 * (fin de jeu, messages serveur) vers les clients graphiques.
 */

#include "gui_protocol.h"

/**
 * @brief Notifie les clients graphiques de la fin de la partie.
 *
 * Envoie le message `seg <team_name>\n`, indiquant que l’équipe a gagné.
 *
 * @param server Le serveur.
 * @param team_name Le nom de l’équipe gagnante.
 */
void gui_broadcast_game_end(server_t *server, const char *team_name)
{
    char message[MAX_GUI_RESPONSE];

    if (!team_name) {
        return;
    }
    snprintf(message, sizeof(message), "seg %s\n", team_name);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Envoie un message serveur générique aux clients graphiques.
 *
 * Envoie le message `smg <server_msg>\n`, qui peut contenir n’importe quelle information.
 *
 * @param server Le serveur.
 * @param server_msg Le message à diffuser.
 */
void gui_broadcast_server_message(server_t *server, const char *server_msg)
{
    char message[MAX_GUI_RESPONSE];

    if (!server_msg) {
        return;
    }
    snprintf(message, sizeof(message), "smg %s\n", server_msg);
    gui_send_to_all_graphic_clients(server, message);
}
