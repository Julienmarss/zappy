/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation_requirements
*/

/**
 * @file incantation_requirements.c
 * @brief Gère les vérifications des prérequis pour effectuer une incantation.
 */

#include "incantation.h"

/// Tableau des prérequis pour chaque niveau d'incantation (niveau, nb joueurs, ressources)
static const elevation_req_t ELEVATION_REQUIREMENTS[] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 0, 0, 0},
    {3, 2, 2, 0, 1, 0, 2, 0},
    {4, 4, 1, 1, 2, 0, 1, 0},
    {5, 4, 1, 2, 1, 3, 0, 0},
    {6, 6, 1, 2, 3, 0, 1, 0},
    {7, 6, 2, 2, 2, 2, 2, 1}
};

/**
 * @brief Compte les joueurs de même niveau présents sur la case du joueur.
 *
 * @param server Le serveur du jeu.
 * @param player Le joueur initiateur.
 * @return Le nombre de joueurs de même niveau sur la case.
 */
static int count_same_level_players(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int count = 0;

    if (!tile) {
        return 0;
    }
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i]->level == player->level) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Remplit le contexte d'incantation avec les participants (même case, même niveau).
 *
 * @param server Le serveur.
 * @param player Le joueur initiateur.
 * @param ctx Le contexte d’incantation à remplir.
 */
static void collect_participants(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    int count = 0;

    if (!tile) {
        return;
    }
    for (int i = 0; i < tile->player_count && count < MAX_PARTICIPANTS; i++) {
        if (tile->players[i]->level == player->level) {
            ctx->participants[count] = tile->players[i];
            count++;
        }
    }
    ctx->nb_participants = count;
}

/**
 * @brief Vérifie qu'une ressource est présente en quantité suffisante.
 *
 * @param tile La case à vérifier.
 * @param resource_type Le type de ressource.
 * @param required_amount Quantité requise.
 * @return true si suffisant, false sinon.
 */
static bool check_single_resource(tile_t *tile, int resource_type,
    int required_amount)
{
    return tile->resources[resource_type] >= required_amount;
}

/**
 * @brief Vérifie que les pointeurs de la case et des prérequis sont valides.
 *
 * @param tile La case du jeu.
 * @param req Les prérequis d'incantation.
 * @return true si valides, false sinon.
 */
static bool validate_tile_and_requirements(tile_t *tile,
    const elevation_req_t *req)
{
    return tile != NULL && req != NULL;
}

/**
 * @brief Vérifie que toutes les ressources nécessaires sont présentes sur la case.
 *
 * @param tile La case à vérifier.
 * @param req Les ressources requises.
 * @return true si toutes les ressources sont disponibles, false sinon.
 */
static bool check_all_resources(tile_t *tile, const elevation_req_t *req)
{
    if (!check_single_resource(tile, LINEMATE, req->linemate)) return false;
    if (!check_single_resource(tile, DERAUMERE, req->deraumere)) return false;
    if (!check_single_resource(tile, SIBUR, req->sibur)) return false;
    if (!check_single_resource(tile, MENDIANE, req->mendiane)) return false;
    if (!check_single_resource(tile, PHIRAS, req->phiras)) return false;
    if (!check_single_resource(tile, THYSTAME, req->thystame)) return false;
    return true;
}

/**
 * @brief Récupère les prérequis d'incantation pour un niveau donné.
 *
 * @param level Le niveau actuel du joueur.
 * @return Un pointeur vers les prérequis correspondants, ou NULL si niveau invalide.
 */
const elevation_req_t *get_elevation_requirements(int level)
{
    if (level < 1 || level > 7) {
        return NULL;
    }
    return &ELEVATION_REQUIREMENTS[level - 1];
}

/**
 * @brief Vérifie si le joueur remplit les conditions en nombre de joueurs pour incanter.
 *
 * @param server Le serveur.
 * @param player Le joueur initiateur.
 * @param ctx Le contexte à remplir si réussite.
 * @return true si les conditions sont remplies, false sinon.
 */
bool check_player_requirements(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    const elevation_req_t *req = get_elevation_requirements(player->level);
    int same_level_count = count_same_level_players(server, player);

    if (!req) return false;
    if (same_level_count < req->nb_players) return false;

    ctx->participants = malloc(sizeof(player_t *) * MAX_PARTICIPANTS);
    if (!ctx->participants) return false;

    collect_participants(server, player, ctx);
    return true;
}

/**
 * @brief Vérifie si les ressources requises sont présentes sur la case du joueur.
 *
 * @param server Le serveur.
 * @param player Le joueur concerné.
 * @return true si les ressources sont suffisantes, false sinon.
 */
bool check_resource_requirements(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    const elevation_req_t *req = get_elevation_requirements(player->level);

    if (!validate_tile_and_requirements(tile, req)) {
        return false;
    }
    return check_all_resources(tile, req);
}
