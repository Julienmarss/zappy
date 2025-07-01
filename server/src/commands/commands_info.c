/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_info
*/

/**
 * @file commands_info.c
 * @brief Implémente les commandes `Inventory` et `Connect_nbr` pour les clients joueurs.
 */

#include "server.h"

/// Noms lisibles des différentes ressources.
static const char *RESOURCE_NAMES[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

/**
 * @brief Commande `Inventory` : envoie l'inventaire du joueur au format Zappy.
 *
 * Format retourné : `[resource count, ...]\n`
 *
 * @param server Le serveur (non utilisé ici).
 * @param client Le client ayant envoyé la commande.
 * @param args Les arguments de la commande (non utilisés).
 */
void cmd_inventory(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    char buffer[512] = "[";
    char tmp[64];

    (void)server;
    (void)args;
    if (!player) {
        network_send(client, "ko\n");
        return;
    }
    for (int i = 0; i < NB_RESOURCES; i++) {
        snprintf(tmp, sizeof(tmp), "%s %d",
            RESOURCE_NAMES[i], player->inventory[i]);
        strcat(buffer, tmp);
        if (i < NB_RESOURCES - 1)
            strcat(buffer, ", ");
    }
    strcat(buffer, "]\n");
    printf("DEBUG: Inventory: %s", buffer);
    network_send(client, buffer);
}

/**
 * @brief Commande `Connect_nbr` : renvoie le nombre de connexions disponibles dans l'équipe du joueur.
 *
 * Format retourné : `<nb_slots_libres>\n`
 *
 * @param server Le serveur (non utilisé ici).
 * @param client Le client ayant envoyé la commande.
 * @param args Les arguments de la commande (non utilisés).
 */
void cmd_connect_nbr(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    team_t *team = NULL;
    char buffer[32];

    (void)server;
    (void)args;
    if (!player || !player->team) {
        network_send(client, "ko\n");
        return;
    }
    team = player->team;
    snprintf(buffer, sizeof(buffer), "%d\n",
        team->max_clients - team->connected_clients);
    printf("DEBUG: Connect_nbr: %s", buffer);
    network_send(client, buffer);
}
