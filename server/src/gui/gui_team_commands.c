/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_team_commands
*/

/**
 * @file gui_team_commands.c
 * @brief Commande GUI `tna` — Envoie les noms de toutes les équipes.
 *
 * Ce fichier implémente la commande `tna`, qui permet aux clients graphiques
 * d’obtenir la liste des noms des équipes existantes.
 */

#include "gui_protocol.h"

/**
 * @brief Envoie le nom d’une équipe à un client graphique.
 *
 * @param client Le client graphique.
 * @param team_name Le nom de l’équipe à envoyer.
 */
static void send_single_team_name(client_t *client, const char *team_name)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "tna %s\n", team_name);
    network_send(client, response);
    printf("DEBUG: Sent team name: %s\n", team_name);
}

/**
 * @brief Commande GUI `tna` — Transmet la liste de toutes les équipes.
 *
 * Format de réponse pour chaque équipe :
 * ```
 * tna <team_name>\n
 * ```
 *
 * Cette commande est utilisée par l’interface graphique pour obtenir
 * les noms de toutes les équipes existantes dans la partie.
 *
 * @param server Le serveur principal.
 * @param client Le client graphique ayant envoyé la commande.
 * @param args Arguments de la commande (ignorés ici).
 */
void gui_cmd_tna(server_t *server, client_t *client, char **args)
{
    team_t *team = NULL;
    int team_count = 0;

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Sending all team names to GUI\n");
    team = server->game->teams;
    while (team) {
        send_single_team_name(client, team->name);
        team_count++;
        team = team->next;
    }
    printf("DEBUG: Sent %d team names\n", team_count);
}
