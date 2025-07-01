/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_formatter
*/

/**
 * @file vision_formatter.c
 * @brief Formate la réponse de la commande de vision d’un joueur en une chaîne lisible.
 */

#include "vision.h"

/**
 * @brief Formate une seule case de vision et l'ajoute au buffer.
 *
 * @param format Structure contenant les infos de formatage.
 * @param tile_index Index de la case dans la vision.
 * @param buffer Le buffer où stocker la réponse.
 * @param first Booléen pour savoir s'il s'agit de la première case.
 */
static void format_single_tile(vision_format_t *format, int tile_index,
    char *buffer, bool *first)
{
    char tile_content[512];
    int world_x = 0;
    int world_y = 0;

    if (!*first)
        strcat(buffer, ",");
    vision_get_world_coordinates(format, tile_index, &world_x, &world_y);
    vision_format_tile_content(format->server, world_x, world_y,
        tile_content);
    strcat(buffer, tile_content);
    *first = false;
}

/**
 * @brief Ajoute le crochet ouvrant au début du buffer (`[`).
 *
 * @param buffer Le buffer de sortie.
 */
static void add_opening_bracket(char *buffer)
{
    strcpy(buffer, "[");
}

/**
 * @brief Ajoute le crochet fermant à la fin du buffer (`]`).
 *
 * @param buffer Le buffer de sortie.
 */
static void add_closing_bracket(char *buffer)
{
    strcat(buffer, "]");
}

/**
 * @brief Traite toutes les cases visibles et les ajoute au buffer formaté.
 *
 * @param format Structure contenant les données de vision.
 * @param buffer Le buffer de sortie.
 */
static void process_all_tiles(vision_format_t *format, char *buffer)
{
    bool first = true;

    for (int i = 0; i < format->data->tile_count; i++)
        format_single_tile(format, i, buffer, &first);
}

/**
 * @brief Génère la chaîne complète représentant le champ de vision d’un joueur.
 *
 * Le format est de type `[content_tile_0,content_tile_1,...,content_tile_n]`
 *
 * @param format Structure de formatage contenant le contexte.
 * @param buffer Le buffer où sera stockée la chaîne finale.
 */
void vision_format_response(vision_format_t *format, char *buffer)
{
    add_opening_bracket(buffer);
    process_all_tiles(format, buffer);
    add_closing_bracket(buffer);
}
