/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_set
*/

/**
 * @file commands_set.c
 * @brief Implémente la commande `Set` permettant à un joueur de poser une ressource au sol.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Valide la requête `Set`.
 *
 * Vérifie que le joueur existe et que la ressource spécifiée est valide.
 *
 * @param client Le client ayant envoyé la commande.
 * @param args Les arguments de la commande.
 * @return true si la requête est valide, false sinon.
 */
static bool validate_set_request(client_t *client, char **args)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (!args[1] || !is_valid_resource_name(args[1])) {
        printf("DEBUG: Set - Invalid resource name: %s\n",
            args[1] ? args[1] : "NULL");
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

/**
 * @brief Vérifie si le joueur possède la ressource à poser.
 *
 * @param player Le joueur concerné.
 * @param resource_name Le nom de la ressource.
 * @return true si le joueur en possède au moins une unité, false sinon.
 */
static bool check_inventory_availability(player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);

    if (player->inventory[resource_index] <= 0) {
        printf("DEBUG: Set - Player %d has no %s in inventory\n",
            player->id, resource_name);
        return false;
    }
    return true;
}

/**
 * @brief Exécute la logique du `Set`.
 *
 * Retire la ressource de l'inventaire et l'ajoute à la tuile du joueur.
 *
 * @param server Le serveur.
 * @param player Le joueur.
 * @param resource_name Nom de la ressource à poser.
 */
static void execute_set_action(server_t *server, player_t *player,
    const char *resource_name)
{
    int resource_index = get_resource_index_by_name(resource_name);
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    player->inventory[resource_index]--;
    tile->resources[resource_index]++;
    printf("DEBUG: Player %d set %s at (%d,%d), inventory now: %d\n",
        player->id, resource_name, player->x, player->y,
        player->inventory[resource_index]);
}

/**
 * @brief Notifie les clients graphiques d’un changement sur une tuile.
 *
 * @param server Le serveur.
 * @param player Le joueur ayant modifié la tuile.
 */
static void broadcast_tile_content_change(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    char message[MAX_GUI_RESPONSE];

    if (!tile)
        return;
    snprintf(message, sizeof(message),
        "bct %d %d %d %d %d %d %d %d %d\n",
        player->x, player->y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    gui_send_to_all_graphic_clients(server, message);
}

/**
 * @brief Commande `Set` : permet à un joueur de déposer une ressource sur la tuile actuelle.
 *
 * @param server Le serveur.
 * @param client Le client ayant émis la commande.
 * @param args Arguments de la commande, où args[1] est le nom de la ressource.
 */
void cmd_set(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char *resource_name = args[1];
    int resource_index = 0;

    if (!validate_set_request(client, args))
        return;
    if (!check_inventory_availability(player, resource_name)) {
        network_send(client, "ko\n");
        return;
    }
    resource_index = get_resource_index_by_name(resource_name);
    execute_set_action(server, player, resource_name);
    network_send(client, "ok\n");
    gui_broadcast_resource_drop(server, player, resource_index);
    gui_broadcast_player_inventory(server, player);
    broadcast_tile_content_change(server, player);
}
