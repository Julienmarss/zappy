/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_create
*/

/**
 * @file game_create.c
 * @brief Implémente la création et l'initialisation du monde de jeu (map, ressources, équipes).
 */

#include "server.h"

/**
 * @brief Densité initiale de chaque type de ressource par case.
 */
static const float RESOURCE_DENSITY[] = {
    0.5,  // FOOD
    0.3,  // LINEMATE
    0.15, // DERAUMERE
    0.1,  // SIBUR
    0.1,  // MENDIANE
    0.08, // PHIRAS
    0.05  // THYSTAME
};

/**
 * @brief Libère une carte partiellement allouée en cas d'échec.
 *
 * @param map La carte à libérer.
 * @param allocated_rows Nombre de lignes déjà allouées.
 */
static void cleanup_partial_map(tile_t **map, int allocated_rows)
{
    for (int i = 0; i < allocated_rows; i++)
        free(map[i]);
    free(map);
}

/**
 * @brief Alloue une ligne de la carte.
 *
 * @param map La carte.
 * @param y Index de ligne.
 * @param width Largeur de la carte.
 * @return true en cas de succès, false sinon.
 */
static bool allocate_map_row(tile_t **map, int y, int width)
{
    map[y] = calloc(width, sizeof(tile_t));
    if (!map[y])
        return false;
    for (int x = 0; x < width; x++)
        map[y][x].players = calloc(MAX_CLIENTS, sizeof(player_t *));
    return true;
}

/**
 * @brief Initialise les données du jeu avec les paramètres donnés.
 *
 * @param game Instance de jeu à initialiser.
 * @param params Paramètres du jeu (dimensions, fréquence, etc.).
 * @return true en cas de succès, false sinon.
 */
static bool initialize_game_data(game_t *game, game_params_t *params)
{
    game->width = params->width;
    game->height = params->height;
    game->time_unit = params->freq;
    game->map = create_map(params->width, params->height);
    return (game->map != NULL);
}

/**
 * @brief Fait apparaître une certaine quantité d’un type de ressource aléatoirement sur la carte.
 *
 * @param game Instance du jeu.
 * @param resource_type Type de ressource à générer.
 * @param quantity Quantité à générer.
 */
static void spawn_single_resource_type(game_t *game, int resource_type,
    int quantity)
{
    tile_t *tile = NULL;

    for (int j = 0; j < quantity; j++) {
        tile = game_get_tile(game, rand() % game->width,
            rand() % game->height);
        if (tile)
            tile->resources[resource_type]++;
    }
}

/**
 * @brief Génère les ressources initiales sur la carte en fonction de leur densité.
 *
 * @param game Instance du jeu.
 */
static void spawn_initial_resources(game_t *game)
{
    int total_tiles = game->width * game->height;
    int quantity = 0;

    for (int i = 0; i < NB_RESOURCES; i++) {
        quantity = (int)(total_tiles * RESOURCE_DENSITY[i]);
        if (quantity < 1)
            quantity = 1;
        spawn_single_resource_type(game, i, quantity);
    }
    game->last_resource_spawn = get_time_microseconds();
}

/**
 * @brief Crée une carte du monde allouée dynamiquement.
 *
 * @param width Largeur de la carte.
 * @param height Hauteur de la carte.
 * @return Un tableau 2D de `tile_t*` représentant la carte, ou NULL en cas d’échec.
 */
tile_t **create_map(int width, int height)
{
    tile_t **map = calloc(height, sizeof(tile_t *));

    if (!map)
        return NULL;
    for (int y = 0; y < height; y++) {
        if (!allocate_map_row(map, y, width)) {
            cleanup_partial_map(map, y);
            return NULL;
        }
    }
    return map;
}

/**
 * @brief Crée et initialise une nouvelle instance de jeu.
 *
 * @param params Paramètres à appliquer pour la création du jeu.
 * @return Un pointeur vers l'objet `game_t`, ou NULL en cas d'échec.
 */
game_t *game_create(game_params_t *params)
{
    game_t *game = calloc(1, sizeof(game_t));

    if (!game)
        return NULL;
    if (!initialize_game_data(game, params)) {
        free(game);
        return NULL;
    }
    init_teams(game, params->teams, params->nb_clients);
    spawn_initial_resources(game);
    return game;
}
