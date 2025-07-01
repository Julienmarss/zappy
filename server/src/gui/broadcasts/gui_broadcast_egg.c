/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_egg
*/

/**
 * @file gui_broadcast_egg.c
 * @brief Fonctions de broadcast des événements liés aux œufs (ponte, connexion, mort)
 * vers les clients graphiques.
 */

#include "gui_protocol.h"

/**
 * @brief Notifie les clients graphiques qu’un œuf a été pondu.
 *
 * Envoie le message `enw #<egg_id> #<player_id> <x> <y>\n`.
 *
 * @param server Le serveur.
 * @param egg L'œuf pondu.
 * @param player Le joueur ayant pondu l'œuf.
 */
void gui_broadcast_egg_laid(server_t *server, egg_t *egg, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg || !player) {
        return;
    }
    snprintf(message, sizeof(message), "enw #%d #%d %d %d\n",
        egg->id, player->id, egg->x, egg->y);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Notifie les clients graphiques qu’un œuf s’est connecté (un joueur a éclos).
 *
 * Envoie le message `ebo #<egg_id>\n`.
 *
 * @param server Le serveur.
 * @param egg L'œuf concerné.
 */
void gui_broadcast_egg_connection(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        return;
    }
    snprintf(message, sizeof(message), "ebo #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Notifie les clients graphiques qu’un œuf est mort (non utilisé ou expiré).
 *
 * Envoie le message `edi #<egg_id>\n`.
 *
 * @param server Le serveur.
 * @param egg L'œuf concerné.
 */
void gui_broadcast_egg_death(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        return;
    }
    snprintf(message, sizeof(message), "edi #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
}
