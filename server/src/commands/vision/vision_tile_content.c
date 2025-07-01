/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_tile_content
*/

/**
 * @file vision_tile_content.c
 * @brief Génère la chaîne représentant le contenu d’une case (joueurs + ressources).
 */

#include "vision.h"

/**
 * @brief Tableau des noms de ressources dans l’ordre correspondant à l’énumération.
 */
static const char *RESOURCE_NAMES[] = {
    "food", "linemate", "deraumere", "sibur",
    "mendiane", "phiras", "thystame"
};

/**
 * @brief Ajoute un joueur à la chaîne du buffer (séparé par un espace si nécessaire).
 *
 * @param ctx Le contexte d’ajout (buffer + booléen de séparation).
 */
static void add_player_to_buffer(content_context_t *ctx)
{
    if (!*ctx->first)
        strcat(ctx->buffer, " ");
    strcat(ctx->buffer, "player");
    *ctx->first = false;
}

/**
 * @brief Ajoute tous les joueurs présents sur une case au buffer.
 *
 * @param server Le serveur contenant l’état des joueurs.
 * @param x Coordonnée X de la case.
 * @param y Coordonnée Y de la case.
 * @param ctx Le contexte d’ajout.
 */
static void add_players_to_content(server_t *server, int x, int y,
    content_context_t *ctx)
{
    int player_count = vision_count_players_at_position(server, x, y);

    for (int i = 0; i < player_count; i++)
        add_player_to_buffer(ctx);
}

/**
 * @brief Ajoute une ressource nommée au buffer (séparé par un espace si nécessaire).
 *
 * @param ctx Le contexte d’ajout.
 * @param name Le nom de la ressource à ajouter.
 */
static void add_resource_to_buffer(content_context_t *ctx, const char *name)
{
    if (!*ctx->first)
        strcat(ctx->buffer, " ");
    strcat(ctx->buffer, name);
    *ctx->first = false;
}

/**
 * @brief Ajoute toutes les ressources présentes sur une case au buffer.
 *
 * @param tile La case contenant les ressources.
 * @param ctx Le contexte d’ajout.
 */
static void add_resources_to_content(tile_t *tile, content_context_t *ctx)
{
    for (int i = 0; i < NB_RESOURCES; i++) {
        for (int count = 0; count < tile->resources[i]; count++)
            add_resource_to_buffer(ctx, RESOURCE_NAMES[i]);
    }
}

/**
 * @brief Formate le contenu d’une case (joueurs + ressources) en une seule chaîne.
 *
 * Le format est de type `"player food food sibur"` selon le contenu de la case.
 *
 * @param server Le serveur de jeu.
 * @param x Coordonnée X de la case.
 * @param y Coordonnée Y de la case.
 * @param buffer Le buffer à remplir (doit être assez grand).
 */
void vision_format_tile_content(server_t *server, int x, int y, char *buffer)
{
    tile_t *tile = game_get_tile(server->game, x, y);
    bool first = true;
    content_context_t ctx = {buffer, &first};

    buffer[0] = '\0';
    if (!tile)
        return;
    add_players_to_content(server, x, y, &ctx);
    add_resources_to_content(tile, &ctx);
}
