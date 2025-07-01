/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_gui
*/

/**
 * @file network_gui.c
 * @brief Gestion de la connexion des clients graphiques (GUI) pour le serveur Zappy.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Envoie les informations initiales de la carte au client graphique.
 *
 * @param server Le serveur.
 * @param client Le client graphique.
 */
static void send_initial_map_data(server_t *server, client_t *client)
{
    gui_cmd_msz(server, client, NULL);  // Taille de la carte
    gui_cmd_mct(server, client, NULL);  // Contenu de toute la carte
    gui_cmd_tna(server, client, NULL);  // Noms des équipes
}

/**
 * @brief Envoie les informations des joueurs existants à tous les clients graphiques.
 *
 * @param server Le serveur.
 */
static void send_initial_player_data(server_t *server)
{
    client_t *c = server->clients;

    while (c) {
        if (c->type == CLIENT_PLAYER && c->player) {
            gui_broadcast_new_player(server, c->player);
            gui_broadcast_player_position(server, c->player);
            gui_broadcast_player_level(server, c->player);
            gui_broadcast_player_inventory(server, c->player);
        }
        c = c->next;
    }
}

/**
 * @brief Envoie l'unité de temps courante au client graphique.
 *
 * @param server Le serveur.
 * @param client Le client graphique.
 */
static void send_time_unit(server_t *server, client_t *client)
{
    gui_cmd_sgt(server, client, NULL);
}

/**
 * @brief Initialise une nouvelle connexion d’un client graphique.
 *
 * @param server Le serveur.
 * @param client Le client graphique.
 */
void network_handle_graphic_connection(server_t *server, client_t *client)
{
    printf("DEBUG: Setting up graphic client connection\n");
    client->type = CLIENT_GRAPHIC;
    printf("DEBUG: Sending initial map data\n");
    send_initial_map_data(server, client);
    printf("DEBUG: Sending initial player data\n");
    send_initial_player_data(server);
    printf("DEBUG: Sending time unit\n");
    send_time_unit(server, client);
    printf("DEBUG: Graphic client setup complete\n");
}
