/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_client
*/

/**
 * @file network_client.c
 * @brief Gère la connexion des nouveaux clients au serveur, qu'ils soient graphiques ou joueurs.
 */

#include "server.h"

/**
 * @brief Nettoie une chaîne en supprimant les caractères de saut de ligne, retour chariot, et null terminator.
 *
 * @param str La chaîne brute à nettoyer.
 * @return Une chaîne nettoyée statiquement allouée.
 */
static char *clean_string_simple(const char *str)
{
    static char buffer[256];
    int len = 0;
    int i = 0;

    if (!str)
        return NULL;
    memset(buffer, 0, sizeof(buffer));
    len = strlen(str);
    for (i = 0; i < len && i < 255; i++) {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != '\0') {
            buffer[i] = str[i];
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    return buffer;
}

/**
 * @brief Cherche une équipe par son nom dans le jeu.
 *
 * @param game Pointeur vers le jeu contenant la liste des équipes.
 * @param name Nom de l'équipe à chercher.
 * @return Pointeur vers l'équipe si trouvée, NULL sinon.
 */
static team_t *find_team(game_t *game, const char *name)
{
    team_t *team = game->teams;

    while (team) {
        if (strcmp(team->name, name) == 0)
            return team;
        team = team->next;
    }
    return NULL;
}

/**
 * @brief Gère la tentative de connexion d’un client à une équipe.
 *
 * @param server Pointeur vers le serveur.
 * @param client Pointeur vers le client à connecter.
 * @param clean_name Nom d’équipe nettoyé.
 */
static void handle_team_connection(server_t *server, client_t *client,
    const char *clean_name)
{
    team_t *team = find_team(server->game, clean_name);

    if (!team) {
        network_reject_connection(server, client);
        return;
    }
    network_handle_player_connection(server, client, team);
}

/**
 * @brief Gère l’arrivée d’un nouveau client, que ce soit un joueur ou un client graphique.
 *
 * @param server Le serveur recevant la connexion.
 * @param client Le client qui tente de se connecter.
 * @param team_name Le nom de l’équipe ou "GRAPHIC" pour les clients graphiques.
 */
void network_handle_new_client(server_t *server, client_t *client,
    const char *team_name)
{
    char *clean_name = clean_string_simple(team_name);

    if (strcmp(clean_name, "GRAPHIC") == 0) {
        network_handle_graphic_connection(server, client);
        return;
    }
    handle_team_connection(server, client, clean_name);
}
