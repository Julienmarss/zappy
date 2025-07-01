/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_time_commands
*/

/**
 * @file gui_time_commands.c
 * @brief Commandes GUI liées au temps (`sgt`, `sst`) pour Zappy.
 *
 * Ce fichier implémente les commandes permettant de consulter et de modifier
 * l'unité de temps du jeu via le protocole graphique.
 */

#include "gui_protocol.h"

/**
 * @brief Commande GUI `sgt` — Envoie l'unité de temps actuelle au client graphique.
 *
 * Format de réponse :
 * ```
 * sgt <time_unit>\n
 * ```
 *
 * @param server Le serveur de jeu.
 * @param client Le client graphique demandeur.
 * @param args Arguments de la commande (ignorés ici).
 */
void gui_cmd_sgt(server_t *server, client_t *client, char **args)
{
    char response[MAX_GUI_RESPONSE];

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    snprintf(response, sizeof(response), "sgt %d\n", server->game->time_unit);
    network_send(client, response);
    printf("DEBUG: Sent time unit to GUI: %d\n", server->game->time_unit);
}

/**
 * @brief Vérifie si l’unité de temps est dans une plage valide (2–10000).
 *
 * @param time_unit Nouvelle unité de temps proposée.
 * @return true si valide, false sinon.
 */
static bool validate_time_unit(int time_unit)
{
    if (time_unit < 2 || time_unit > 10000) {
        printf("DEBUG: Invalid time unit: %d (must be 2-10000)\n", time_unit);
        return false;
    }
    return true;
}

/**
 * @brief Commande GUI `sst` — Modifie l’unité de temps du jeu.
 *
 * Format attendu :
 * ```
 * sst <new_time_unit>\n
 * ```
 * Format de réponse :
 * ```
 * sst <new_time_unit>\n
 * ```
 *
 * @param server Le serveur de jeu.
 * @param client Le client graphique demandeur.
 * @param args Arguments de la commande (doit contenir une valeur entière).
 */
void gui_cmd_sst(server_t *server, client_t *client, char **args)
{
    int new_time_unit = 0;
    char response[MAX_GUI_RESPONSE];

    if (!args[1]) {
        gui_send_bad_parameters(client);
        return;
    }
    new_time_unit = atoi(args[1]);
    if (!validate_time_unit(new_time_unit)) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Changing time unit from %d to %d\n",
        server->game->time_unit, new_time_unit);
    server->game->time_unit = new_time_unit;
    snprintf(response, sizeof(response), "sst %d\n", new_time_unit);
    network_send(client, response);
    printf("DEBUG: Time unit changed successfully to %d\n", new_time_unit);
}
