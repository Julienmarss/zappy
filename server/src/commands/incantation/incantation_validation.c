/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation_validation
*/

/**
 * @file incantation_validation.c
 * @brief Vérifie si un joueur peut lancer une incantation (niveau, conditions, ressources).
 */

#include "incantation.h"

/**
 * @brief Vérifie si un joueur est déjà au niveau maximal.
 *
 * @param level Le niveau actuel du joueur.
 * @return true si le joueur est au niveau maximal, false sinon.
 */
static bool is_player_at_max_level(int level)
{
    return level >= MAX_LEVEL;
}

/**
 * @brief Envoie un message d'erreur ("ko") au client.
 *
 * @param client Le client à notifier.
 */
static void send_validation_error(client_t *client)
{
    network_send(client, "ko\n");
}

/**
 * @brief Valide si le client est un joueur valide et s’il peut encore monter de niveau.
 *
 * @param client Le client à vérifier.
 * @return true si le joueur est valide pour une incantation, false sinon.
 */
bool validate_incantation_player(client_t *client)
{
    if (!client->player) {
        send_validation_error(client);
        return false;
    }
    if (is_player_at_max_level(client->player->level)) {
        send_validation_error(client);
        return false;
    }
    return true;
}

/**
 * @brief Vérifie si toutes les conditions sont réunies pour commencer une incantation :
 *        - Joueurs en nombre suffisant
 *        - Ressources suffisantes sur la case
 *
 * @param server Le serveur du jeu.
 * @param player Le joueur initiateur de l’incantation.
 * @param ctx Le contexte d’incantation à remplir.
 * @return true si les conditions sont remplies, false sinon.
 */
bool check_incantation_conditions(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    ctx->level = player->level;
    ctx->x = player->x;
    ctx->y = player->y;

    if (!check_player_requirements(server, player, ctx)) {
        send_validation_error(player->client);
        return false;
    }
    if (!check_resource_requirements(server, player)) {
        send_validation_error(player->client);
        return false;
    }
    return true;
}
