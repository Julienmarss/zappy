/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_eject
*/

/**
 * @file command_eject.c
 * @brief Gestion de la commande d’éjection (`eject`) pour le serveur Zappy.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Calcule le déplacement à effectuer selon l’orientation du joueur.
 *
 * @param orientation Orientation du joueur (NORTH, SOUTH, EAST, WEST).
 * @param dx Pointeur vers le delta X à remplir.
 * @param dy Pointeur vers le delta Y à remplir.
 */
static void calculate_eject_direction(int orientation, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    if (orientation == NORTH) {
        *dy = -1;
        return;
    }
    if (orientation == SOUTH) {
        *dy = 1;
        return;
    }
    if (orientation == EAST) {
        *dx = 1;
        return;
    }
    if (orientation == WEST)
        *dx = -1;
}

/**
 * @brief Retourne l’orientation opposée à celle fournie.
 *
 * @param orientation Orientation d’origine.
 * @return Orientation opposée.
 */
static int get_opposite_direction(int orientation)
{
    if (orientation == NORTH)
        return SOUTH;
    if (orientation == SOUTH)
        return NORTH;
    if (orientation == EAST)
        return WEST;
    return EAST;
}

/**
 * @brief Envoie une notification `eject: <dir>` au joueur éjecté.
 *
 * @param target Joueur éjecté.
 * @param opposite_dir Direction d'où il a été éjecté.
 */
static void send_eject_notification(player_t *target, int opposite_dir)
{
    char message[32];

    if (!target->client)
        return;
    snprintf(message, sizeof(message), "eject: %d\n", opposite_dir);
    network_send(target->client, message);
}

/**
 * @brief Déplace physiquement un joueur vers une nouvelle tuile.
 *
 * Supprime le joueur de l'ancienne tuile et l'ajoute à la nouvelle.
 *
 * @param server Le serveur contenant le jeu.
 * @param player Le joueur à déplacer.
 * @param new_x Nouvelle coordonnée X.
 * @param new_y Nouvelle coordonnée Y.
 */
static void move_player_to_position(server_t *server, player_t *player,
    int new_x, int new_y)
{
    remove_player_from_old_tile(server, player);
    player->x = new_x;
    player->y = new_y;
    add_player_to_new_tile(server, player, new_x, new_y);
}

/**
 * @brief Éjecte un joueur cible à partir d’un joueur qui exécute la commande.
 *
 * @param server Le serveur.
 * @param ejector Le joueur qui effectue l’éjection.
 * @param target Le joueur ciblé pour l’éjection.
 */
void eject_single_player(server_t *server, player_t *ejector,
    player_t *target)
{
    int dx = 0;
    int dy = 0;
    int new_x = 0;
    int new_y = 0;
    int opposite_dir = 0;

    calculate_eject_direction(ejector->orientation, &dx, &dy);
    new_x = (target->x + dx + server->game->width) % server->game->width;
    new_y = (target->y + dy + server->game->height) % server->game->height;
    opposite_dir = get_opposite_direction(ejector->orientation);
    move_player_to_position(server, target, new_x, new_y);
    send_eject_notification(target, opposite_dir);
}

/**
 * @brief Commande principale d’éjection (`eject`) exécutée par un joueur.
 *
 * Éjecte tous les autres joueurs sur la même tuile et notifie le client.
 *
 * @param server Le serveur.
 * @param client Le client qui a envoyé la commande.
 * @param args Arguments de la commande (ignorés ici).
 */
void cmd_eject(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    int ejected_count = 0;

    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    ejected_count = eject_other_players(server, player);
    destroy_eggs_on_tile(server, player->x, player->y);
    if (ejected_count > 0)
        gui_broadcast_player_expulsion(server, player);
    network_send(client, "ok\n");
}
