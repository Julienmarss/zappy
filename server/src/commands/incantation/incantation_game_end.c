/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation_game_end
*/

/**
 * @file incantation_game_end.c
 * @brief Contient la logique de fin de partie basée sur les incantations.
 */

#include "incantation.h"
#include "gui_protocol.h"

/**
 * @brief Compte le nombre de joueurs de niveau 8 ou plus dans une équipe.
 *
 * @param server Le serveur contenant les clients et le jeu.
 * @param team L’équipe à vérifier.
 * @return Le nombre de joueurs de niveau 8 ou plus dans l’équipe.
 */
static int count_level_eight_players(server_t *server, team_t *team)
{
    client_t *client = server->clients;
    int count = 0;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player &&
            client->player->team == team && client->player->level >= 8) {
            count++;
        }
        client = client->next;
    }
    return count;
}

/**
 * @brief Détermine si une équipe a gagné (au moins 6 joueurs niveau 8).
 *
 * @param server Le serveur de jeu.
 * @param team L’équipe à vérifier.
 * @return true si l’équipe a gagné, false sinon.
 */
static bool team_has_won(server_t *server, team_t *team)
{
    int level_eight_count = count_level_eight_players(server, team);

    return level_eight_count >= 6;
}

/**
 * @brief Vérifie si une des équipes a gagné la partie et informe les clients graphiques.
 *
 * Une équipe gagne si elle possède au moins 6 joueurs ayant atteint le niveau 8.
 *
 * @param server Le serveur de jeu.
 */
void check_game_end_condition(server_t *server)
{
    team_t *team = server->game->teams;

    while (team) {
        if (team_has_won(server, team)) {
            gui_broadcast_game_end(server, team->name);
            return;
        }
        team = team->next;
    }
}
