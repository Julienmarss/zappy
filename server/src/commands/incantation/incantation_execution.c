/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation_execution
*/

/**
 * @file incantation_execution.c
 * @brief Contient les fonctions de gestion de l'exécution d'une incantation dans Zappy.
 */

#include "incantation.h"
#include "gui_protocol.h"

/**
 * @brief Envoie une mise à jour de l'état de la case (ressources) après une incantation.
 *
 * @param server Le serveur contenant les informations du jeu.
 * @param x La coordonnée X de la case.
 * @param y La coordonnée Y de la case.
 */
static void broadcast_tile_content_after_incantation(server_t *server,
    int x, int y)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    char message[MAX_GUI_RESPONSE];

    if (!tile)
        return;
    snprintf(message, sizeof(message),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
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
 * @brief Consomme les ressources nécessaires à l'incantation sur la case du joueur.
 *
 * @param server Le serveur de jeu.
 * @param player Le joueur qui initie l'incantation.
 */
static void consume_elevation_resources(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);
    const elevation_req_t *req = get_elevation_requirements(player->level);

    if (!tile || !req) {
        return;
    }
    tile->resources[LINEMATE] -= req->linemate;
    tile->resources[DERAUMERE] -= req->deraumere;
    tile->resources[SIBUR] -= req->sibur;
    tile->resources[MENDIANE] -= req->mendiane;
    tile->resources[PHIRAS] -= req->phiras;
    tile->resources[THYSTAME] -= req->thystame;
    broadcast_tile_content_after_incantation(server, player->x, player->y);
}

/**
 * @brief Augmente le niveau d’un seul joueur.
 *
 * @param player Le joueur à faire monter de niveau.
 */
static void elevate_single_player(player_t *player)
{
    char message[64];

    player->level++;
    snprintf(message, sizeof(message),
        "Elevation underway\nCurrent level: %d\n", player->level);
    if (player->client) {
        network_send(player->client, message);
    }
}

/**
 * @brief Monte de niveau tous les joueurs participants à l'incantation.
 *
 * @param ctx Le contexte de l'incantation contenant les participants.
 */
static void elevate_participants(incantation_ctx_t *ctx)
{
    for (int i = 0; i < ctx->nb_participants; i++) {
        if (ctx->participants[i]) {
            elevate_single_player(ctx->participants[i]);
        }
    }
}

/**
 * @brief Diffuse aux clients graphiques les mises à jour des joueurs (niveau et inventaire).
 *
 * @param server Le serveur de jeu.
 * @param ctx Le contexte de l'incantation.
 */
static void broadcast_player_updates(server_t *server, incantation_ctx_t *ctx)
{
    for (int i = 0; i < ctx->nb_participants; i++) {
        if (ctx->participants[i]) {
            gui_broadcast_player_level(server, ctx->participants[i]);
            gui_broadcast_player_inventory(server, ctx->participants[i]);
        }
    }
}

/**
 * @brief Fonction principale exécutant l’incantation : 
 * diffusion des messages graphiques, consommation des ressources,
 * élévation des joueurs et vérification de fin de partie.
 *
 * @param server Le serveur de jeu.
 * @param player Le joueur initiateur de l'incantation.
 * @param ctx Le contexte de l'incantation.
 */
void execute_incantation_ritual(server_t *server, player_t *player,
    incantation_ctx_t *ctx)
{
    incantation_data_t incantation_data = {
        .x = ctx->x,
        .y = ctx->y,
        .level = ctx->level,
        .players = ctx->participants,
        .nb_players = ctx->nb_participants
    };

    gui_broadcast_incantation_start(server, &incantation_data);
    consume_elevation_resources(server, player);
    elevate_participants(ctx);
    gui_broadcast_incantation_end(server, ctx->x, ctx->y, true);
    broadcast_player_updates(server, ctx);
    check_game_end_condition(server);
    if (ctx->participants) {
        free(ctx->participants);
    }
}
