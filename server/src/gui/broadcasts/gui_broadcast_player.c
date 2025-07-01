/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_player
*/

/**
 * @file gui_broadcast_player.c
 * @brief Diffusion des informations liées aux joueurs vers les clients graphiques.
 */

#include "gui_protocol.h"

/**
 * @brief Vérifie si un joueur est valide (non NULL).
 *
 * @param player Le joueur à vérifier.
 * @return true si valide, false sinon.
 */
static bool is_player_valid(player_t *player)
{
    return player != NULL;
}

/**
 * @brief Vérifie si un joueur et son équipe sont valides.
 *
 * @param player Le joueur à vérifier.
 * @return true si joueur et team sont valides, false sinon.
 */
static bool is_player_with_team_valid(player_t *player)
{
    return player != NULL && player->team != NULL;
}

/**
 * @brief Formate le message `pnw` pour un nouveau joueur.
 *
 * @param message Le buffer de destination.
 * @param player Le joueur concerné.
 */
static void format_new_player_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE, "pnw #%d %d %d %d %d %s\n",
        player->id, player->x, player->y, player->orientation,
        player->level, player->team->name);
}

/**
 * @brief Formate le message `ppo` pour la position d'un joueur.
 *
 * @param message Le buffer de destination.
 * @param player Le joueur concerné.
 */
static void format_player_position_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE, "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
}

/**
 * @brief Formate le message `pin` pour l'inventaire d'un joueur.
 *
 * @param message Le buffer de destination.
 * @param player Le joueur concerné.
 */
static void format_player_inventory_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE,
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
}

/**
 * @brief Diffuse le message `pnw` pour signaler un nouveau joueur.
 *
 * @param server Le serveur.
 * @param player Le joueur à annoncer.
 */
void gui_broadcast_new_player(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_with_team_valid(player)) {
        return;
    }
    format_new_player_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse la position d'un joueur (`ppo`).
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 */
void gui_broadcast_player_position(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    format_player_position_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse le niveau d'un joueur (`plv`).
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 */
void gui_broadcast_player_level(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "plv #%d %d\n",
        player->id, player->level);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse l'inventaire d'un joueur (`pin`).
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 */
void gui_broadcast_player_inventory(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    format_player_inventory_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Diffuse la mort d’un joueur (`pdi`).
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 */
void gui_broadcast_player_death(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pdi #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}
