/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_incantation
*/

/**
 * @file commands_incantation.c
 * @brief Implémente la commande `Incantation` pour l'élévation de niveau.
 */

#include "server.h"
#include "gui_protocol.h"
#include "incantation.h"

/**
 * @brief Commande `Incantation` : tente d'élever le joueur au niveau supérieur.
 *
 * Cette fonction :
 * - Vérifie si le joueur est valide pour une incantation.
 * - Vérifie les conditions d'incantation (joueurs et ressources).
 * - Exécute le rituel si toutes les conditions sont remplies.
 *
 * @param server Pointeur vers la structure du serveur.
 * @param client Client ayant envoyé la commande.
 * @param args Arguments de la commande (non utilisés ici).
 */
void cmd_incantation(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    incantation_ctx_t ctx = {0};

    (void)args;
    if (!validate_incantation_player(client)) {
        return;
    }
    if (!check_incantation_conditions(server, player, &ctx)) {
        return;
    }
    execute_incantation_ritual(server, player, &ctx);
}
