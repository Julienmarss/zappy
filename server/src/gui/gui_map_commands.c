/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_map_commands
*/

/**
 * @file gui_map_commands.c
 * @brief Implémente les commandes graphiques liées à la carte (map),
 * notamment pour obtenir sa taille et le contenu des cases.
 */

#include "gui_protocol.h"

/**
 * @brief Commande `msz`: envoie la taille de la carte au client graphique.
 *
 * Format : `msz <width> <height>\n`
 *
 * @param server Le serveur principal.
 * @param client Le client graphique demandeur.
 * @param args Les arguments (ignorés ici).
 */
void gui_cmd_msz(server_t *server, client_t *client, char **args)
{
    char response[MAX_GUI_RESPONSE];

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    snprintf(response, sizeof(response), "msz %d %d\n",
        server->game->width, server->game->height);
    network_send(client, response);
    printf("DEBUG: Sent map size to GUI: %dx%d\n",
        server->game->width, server->game->height);
}

/**
 * @brief Vérifie que les coordonnées d'une tuile sont valides.
 *
 * @param server Le serveur contenant la carte.
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @return true si les coordonnées sont valides, sinon false.
 */
static bool validate_tile_coordinates(server_t *server, int x, int y)
{
    if (x < 0 || x >= server->game->width ||
        y < 0 || y >= server->game->height) {
        printf("DEBUG: Invalid tile coordinates: (%d,%d)\n", x, y);
        return false;
    }
    return true;
}

/**
 * @brief Vérifie les arguments de la commande `bct`.
 *
 * @param args Les arguments à valider.
 * @param client Le client pour renvoyer un message d’erreur si besoin.
 * @return true si les arguments sont valides, sinon false.
 */
static bool validate_bct_args(char **args, client_t *client)
{
    if (!args[1] || !args[2]) {
        gui_send_bad_parameters(client);
        return false;
    }
    return true;
}

/**
 * @brief Envoie le contenu d'une tuile donnée au client graphique.
 *
 * Format : `bct x y q0 q1 q2 q3 q4 q5 q6`
 *
 * @param client Le client graphique.
 * @param tile La tuile concernée.
 * @param x Coordonnée X de la tuile.
 * @param y Coordonnée Y de la tuile.
 */
static void send_tile_content_response(client_t *client, tile_t *tile,
    int x, int y)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    network_send(client, response);
    printf("DEBUG: Sent tile content for (%d,%d)\n", x, y);
}

/**
 * @brief Commande `bct`: envoie le contenu d'une tuile spécifique.
 *
 * @param server Le serveur principal.
 * @param client Le client graphique.
 * @param args Arguments contenant les coordonnées X et Y.
 */
void gui_cmd_bct(server_t *server, client_t *client, char **args)
{
    int x = 0;
    int y = 0;
    tile_t *tile = NULL;

    if (!validate_bct_args(args, client)) {
        return;
    }
    x = atoi(args[1]);
    y = atoi(args[2]);
    if (!validate_tile_coordinates(server, x, y)) {
        gui_send_bad_parameters(client);
        return;
    }
    tile = game_get_tile(server->game, x, y);
    if (!tile) {
        gui_send_bad_parameters(client);
        return;
    }
    send_tile_content_response(client, tile, x, y);
}

/**
 * @brief Envoie le contenu d'une seule tuile dans `mct`.
 *
 * @param server Le serveur principal.
 * @param client Le client graphique.
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 */
static void send_single_tile_content(server_t *server, client_t *client,
    int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    char response[MAX_GUI_RESPONSE];

    if (!tile) {
        return;
    }
    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    network_send(client, response);
}

/**
 * @brief Commande `mct`: envoie le contenu de **toutes** les tuiles de la carte.
 *
 * Utilisé pour l'initialisation complète de la carte côté interface graphique.
 *
 * @param server Le serveur principal.
 * @param client Le client graphique.
 * @param args Ignorés.
 */
void gui_cmd_mct(server_t *server, client_t *client, char **args)
{
    int x = 0;
    int y = 0;

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Sending all tile contents to GUI\n");
    for (y = 0; y < server->game->height; y++) {
        for (x = 0; x < server->game->width; x++) {
            send_single_tile_content(server, client, x, y);
        }
    }
    printf("DEBUG: Sent %d tile contents\n",
        server->game->width * server->game->height);
}
