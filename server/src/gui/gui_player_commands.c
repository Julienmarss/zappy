/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_player_commands
*/

/**
 * @file gui_player_commands.c
 * @brief Commandes GUI pour interroger les informations des joueurs :
 * position (ppo), niveau (plv), inventaire (pin).
 */

#include "gui_protocol.h"

/**
 * @brief Recherche un joueur dans la liste des clients à partir de son ID.
 *
 * @param server Le serveur principal.
 * @param player_id L'identifiant unique du joueur.
 * @return Un pointeur vers le joueur s'il est trouvé, sinon NULL.
 */
static player_t *find_player_by_id(server_t *server, int player_id)
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
 * @brief Vérifie si l'argument fourni pour un ID joueur est valide.
 *
 * @param args Les arguments de la commande.
 * @param client Le client qui a envoyé la commande.
 * @return true si l’argument est correct, false sinon.
 */
static bool validate_player_id_arg(char **args, client_t *client)
{
    if (!args[1]) {
        gui_send_bad_parameters(client);
        return false;
    }
    if (args[1][0] != '#') {
        gui_send_bad_parameters(client);
        return false;
    }
    return true;
}

/**
 * @brief Extrait le joueur à partir des arguments de commande.
 *
 * @param server Le serveur principal.
 * @param client Le client demandeur.
 * @param args Les arguments contenant l’ID du joueur.
 * @return Un pointeur vers le joueur si trouvé, NULL sinon.
 */
static player_t *get_player_from_args(server_t *server, client_t *client,
    char **args)
{
    int player_id = 0;
    player_t *player = NULL;

    if (!validate_player_id_arg(args, client)) {
        return NULL;
    }
    player_id = atoi(args[1] + 1);  // skip '#' prefix
    player = find_player_by_id(server, player_id);
    if (!player) {
        printf("DEBUG: Player ID %d not found\n", player_id);
        gui_send_bad_parameters(client);
        return NULL;
    }
    return player;
}

/**
 * @brief Envoie la position et l’orientation du joueur.
 *
 * Format : `ppo #id x y orientation`
 *
 * @param client Le client graphique.
 * @param player Le joueur concerné.
 */
static void send_player_position_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
    network_send(client, response);
    printf("DEBUG: Sent player position for ID %d: (%d,%d) orient %d\n",
        player->id, player->x, player->y, player->orientation);
}

/**
 * @brief Envoie le niveau du joueur.
 *
 * Format : `plv #id level`
 *
 * @param client Le client graphique.
 * @param player Le joueur concerné.
 */
static void send_player_level_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "plv #%d %d\n",
        player->id, player->level);
    network_send(client, response);
    printf("DEBUG: Sent player level for ID %d: level %d\n",
        player->id, player->level);
}

/**
 * @brief Envoie l’inventaire du joueur.
 *
 * Format : `pin #id x y q0 q1 q2 q3 q4 q5 q6`
 *
 * @param client Le client graphique.
 * @param player Le joueur concerné.
 */
static void send_player_inventory_response(client_t *client, player_t *player)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
    network_send(client, response);
    printf("DEBUG: Sent player inventory for ID %d at (%d,%d)\n",
        player->id, player->x, player->y);
}

/**
 * @brief Commande GUI `ppo`: position et orientation d’un joueur.
 */
void gui_cmd_ppo(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_position_response(client, player);
}

/**
 * @brief Commande GUI `plv`: niveau d’un joueur.
 */
void gui_cmd_plv(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_level_response(client, player);
}

/**
 * @brief Commande GUI `pin`: inventaire d’un joueur.
 */
void gui_cmd_pin(server_t *server, client_t *client, char **args)
{
    player_t *player = get_player_from_args(server, client, args);

    if (!player) {
        return;
    }
    send_player_inventory_response(client, player);
}
