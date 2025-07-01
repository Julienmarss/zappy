/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_resource
*/

/**
 * @file gui_broadcast_resource.c
 * @brief Fonctions de diffusion des interactions des joueurs avec les ressources
 * (prise ou dépôt), vers les clients graphiques.
 */

#include "gui_protocol.h"

/**
 * @brief Diffuse un message indiquant qu’un joueur a déposé une ressource.
 *
 * Envoie une commande `pdr` au format :
 * ```
 * pdr #<player_id> <resource_type>
 * ```
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 * @param resource L'indice de la ressource déposée (0 à NB_RESOURCES-1).
 */
void gui_broadcast_resource_drop(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        return;
    }
    snprintf(message, sizeof(message), "pdr #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse un message indiquant qu’un joueur a ramassé une ressource.
 *
 * Envoie une commande `pgt` au format :
 * ```
 * pgt #<player_id> <resource_type>
 * ```
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 * @param resource L'indice de la ressource ramassée (0 à NB_RESOURCES-1).
 */
void gui_broadcast_resource_collect(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        return;
    }
    snprintf(message, sizeof(message), "pgt #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
}
