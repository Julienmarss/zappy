/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_utils
*/

/**
 * @file gui_utils.c
 * @brief Fonctions utilitaires pour le protocole graphique (GUI) dans Zappy.
 */

#include "gui_protocol.h"

/**
 * @brief Envoie un message à tous les clients graphiques connectés.
 *
 * @param server Pointeur vers la structure du serveur.
 * @param message Message à envoyer (terminé par \n).
 */
void gui_send_to_all_graphic_clients(server_t *server, const char *message)
{
    client_t *client = server->clients;
    int sent_count = 0;

    if (!message) {
        printf("DEBUG: Cannot send message - message is NULL\n");
        return;
    }
    while (client) {
        if (client->type == CLIENT_GRAPHIC) {
            network_send(client, message);
            sent_count++;
        }
        client = client->next;
    }
    if (sent_count > 0) {
        printf("DEBUG: Sent GUI message to %d clients: %.50s%s\n",
            sent_count, message, strlen(message) > 50 ? "..." : "");
    }
}

/**
 * @brief Formate un identifiant de joueur pour les messages GUI.
 *
 * @param player_id L'identifiant du joueur.
 * @return Une chaîne statique du type "#<id>".
 */
char *gui_format_player_id(int player_id)
{
    static char buffer[16];

    snprintf(buffer, sizeof(buffer), "#%d", player_id);
    return buffer;
}

/**
 * @brief Formate un identifiant d'œuf pour les messages GUI.
 *
 * @param egg_id L'identifiant de l'œuf.
 * @return Une chaîne statique du type "#<id>".
 */
char *gui_format_egg_id(int egg_id)
{
    static char buffer[16];

    snprintf(buffer, sizeof(buffer), "#%d", egg_id);
    return buffer;
}

/**
 * @brief Vérifie si des coordonnées sont valides sur la carte.
 *
 * @param server Le serveur contenant les dimensions de la carte.
 * @param x Coordonnée x.
 * @param y Coordonnée y.
 * @return true si les coordonnées sont valides, false sinon.
 */
bool gui_is_valid_coordinates(server_t *server, int x, int y)
{
    if (!server || !server->game)
        return false;
    if (x < 0 || x >= server->game->width)
        return false;
    if (y < 0 || y >= server->game->height)
        return false;
    return true;
}

/**
 * @brief Recherche un joueur par son ID dans la liste des clients.
 *
 * @param server Le serveur contenant les clients.
 * @param player_id L'identifiant du joueur recherché.
 * @return Un pointeur vers le joueur s’il est trouvé, NULL sinon.
 */
static player_t *find_player_in_clients(server_t *server, int player_id)
{
    client_t *client = server->clients;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player &&
            client->player->id == player_id) {
            return client->player;
        }
        client = client->next;
    }
    return NULL;
}

/**
 * @brief Vérifie si un identifiant de joueur est valide.
 *
 * @param server Le serveur contenant les clients.
 * @param player_id Identifiant à vérifier.
 * @return true si un joueur avec cet ID existe, false sinon.
 */
bool gui_is_valid_player_id(server_t *server, int player_id)
{
    if (!server || player_id <= 0)
        return false;
    return find_player_in_clients(server, player_id) != NULL;
}
