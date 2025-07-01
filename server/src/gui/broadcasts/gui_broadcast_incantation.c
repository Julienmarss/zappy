/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_incantation
*/

/**
 * @file gui_broadcast_incantation.c
 * @brief Gère la communication des événements d'incantation vers les clients graphiques.
 */

#include "gui_protocol.h"

/**
 * @brief Ajoute l'identifiant d'un joueur à la chaîne de la liste des joueurs.
 *
 * @param buffer La chaîne de destination.
 * @param player Le joueur à ajouter.
 * @param is_first Indique si c'est le premier joueur (pour éviter les espaces en trop).
 */
static void add_player_to_list(char *buffer, player_t *player, bool is_first)
{
    if (!is_first) {
        strcat(buffer, " ");
    }
    strcat(buffer, gui_format_player_id(player->id));
}

/**
 * @brief Formate une liste de joueurs en une chaîne séparée par des espaces.
 *
 * @param buffer Le buffer où écrire la chaîne.
 * @param players Tableau de pointeurs vers les joueurs.
 * @param nb_players Nombre de joueurs dans le tableau.
 */
static void format_players_list(char *buffer, player_t **players,
    int nb_players)
{
    for (int i = 0; i < nb_players; i++) {
        if (players[i]) {
            add_player_to_list(buffer, players[i], i == 0);
        }
    }
}

/**
 * @brief Envoie le message d'incantation en cours (`pic`) aux clients graphiques.
 *
 * @param server Le serveur.
 * @param data Les données d'incantation.
 * @param players_list Liste formatée des ID de joueurs.
 */
static void send_incantation_message(server_t *server,
    incantation_data_t *data, const char *players_list)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pic %d %d %d %s\n",
        data->x, data->y, data->level, players_list);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse le début d'une incantation à tous les clients graphiques.
 *
 * Le message `pic x y level [#id #id ...]` est envoyé.
 *
 * @param server Le serveur.
 * @param data Les données d'incantation (coordonnées, niveau, joueurs).
 */
void gui_broadcast_incantation_start(server_t *server,
    incantation_data_t *data)
{
    char players_list[512] = "";

    format_players_list(players_list, data->players, data->nb_players);
    send_incantation_message(server, data, players_list);
}

/**
 * @brief Diffuse la fin d'une incantation (réussie ou échouée) aux clients graphiques.
 *
 * Le message `pie x y 1` ou `pie x y 0` est envoyé selon le résultat.
 *
 * @param server Le serveur.
 * @param x Coordonnée X de l'incantation.
 * @param y Coordonnée Y de l'incantation.
 * @param result Résultat de l'incantation (true = succès, false = échec).
 */
void gui_broadcast_incantation_end(server_t *server, int x, int y, bool result)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pie %d %d %d\n",
        x, y, result ? 1 : 0);
    gui_send_to_all_graphic_clients(server, message);
}
