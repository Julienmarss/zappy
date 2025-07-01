/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** vision_utils
*/

/**
 * @file vision_utils.c
 * @brief Fournit des fonctions utilitaires pour la gestion de la vision : comptage des joueurs,
 * transformation des coordonnées locales en coordonnées globales.
 */

#include "vision.h"

/**
 * @brief Vérifie si un client est un joueur positionné à une case donnée.
 *
 * @param client Le client à vérifier.
 * @param x Coordonnée X à tester.
 * @param y Coordonnée Y à tester.
 * @return true si le joueur est à la position, false sinon.
 */
static bool is_player_at_position(client_t *client, int x, int y)
{
    return (client->type == CLIENT_PLAYER && client->player &&
        client->player->x == x && client->player->y == y);
}

/**
 * @brief Calcule la position réelle dans le monde à partir d’un index de case dans le champ de vision.
 *
 * Prend en compte l’orientation du joueur et applique un modulo pour respecter les bords de la map.
 *
 * @param format Structure de formatage (contenant joueur, données et map).
 * @param tile_index Index de la case dans le champ de vision.
 * @param world_x Pointeur vers la coordonnée X calculée.
 * @param world_y Pointeur vers la coordonnée Y calculée.
 */
static void calculate_world_position(vision_format_t *format, int tile_index,
    int *world_x, int *world_y)
{
    vision_transform_t transform = {
        .orientation = format->player->orientation,
        .local_x = format->data->tiles[tile_index].x,
        .local_y = format->data->tiles[tile_index].y
    };
    int dx = 0;
    int dy = 0;

    vision_transform_coordinates(&transform, &dx, &dy);
    *world_x = (format->player->x + dx + format->server->game->width)
        % format->server->game->width;
    *world_y = (format->player->y + dy + format->server->game->height)
        % format->server->game->height;
}

/**
 * @brief Compte le nombre de joueurs présents à une position spécifique sur la map.
 *
 * @param server Le serveur contenant tous les clients.
 * @param x Coordonnée X ciblée.
 * @param y Coordonnée Y ciblée.
 * @return Le nombre de joueurs présents sur cette case.
 */
int vision_count_players_at_position(server_t *server, int x, int y)
{
    client_t *current = server->clients;
    int count = 0;

    while (current) {
        if (is_player_at_position(current, x, y))
            count++;
        current = current->next;
    }
    return count;
}

/**
 * @brief Transforme une position locale (dans le champ de vision) en coordonnées globales.
 *
 * @param format Structure de formatage contenant le joueur et la map.
 * @param tile_index Index de la case dans la pyramide de vision.
 * @param world_x Pointeur vers la coordonnée X résultante.
 * @param world_y Pointeur vers la coordonnée Y résultante.
 */
void vision_get_world_coordinates(vision_format_t *format, int tile_index,
    int *world_x, int *world_y)
{
    calculate_world_position(format, tile_index, world_x, world_y);
}
