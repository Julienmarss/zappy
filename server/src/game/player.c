/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

/**
 * @file player.c
 * @brief Contient les fonctions de gestion des joueurs dans le jeu Zappy.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Crée un nouveau joueur à une position donnée pour une équipe donnée.
 *
 * @param team Pointeur vers l'équipe du joueur.
 * @param x Position X initiale.
 * @param y Position Y initiale.
 * @return Pointeur vers le joueur créé, ou NULL en cas d'erreur.
 */
player_t *player_create(team_t *team, int x, int y)
{
    player_t *player = calloc(1, sizeof(player_t));
    static int id_counter = 0;

    if (!player)
        return NULL;
    id_counter++;
    player->id = id_counter;
    player->x = x;
    player->y = y;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    player->inventory[FOOD] = 10;
    player->food_timer = 126;
    player->team = team;
    player->is_incanting = false;
    player->action_time = 0;
    printf("DEBUG: Created player ID %d (%d,%d), orientation %d, lvl %d\n",
        player->id, x, y, player->orientation, player->level);
    return player;
}

/**
 * @brief Ajoute un joueur sur la tuile correspondante à sa position.
 *
 * @param server Pointeur vers la structure serveur.
 * @param player Pointeur vers le joueur à ajouter.
 */
void player_add_to_tile(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    if (!tile || tile->player_count >= MAX_CLIENTS) {
        printf("DEBUG: Failed to add player to tile (%d,%d)\n",
            player->x, player->y);
        return;
    }
    tile->players[tile->player_count] = player;
    tile->player_count++;
    printf("DEBUG: Added player ID %d to tile (%d,%d), count: %d\n",
        player->id, player->x, player->y, tile->player_count);
}

/**
 * @brief Décale les joueurs dans la tuile à partir d'un index pour en supprimer un.
 *
 * @param tile Pointeur vers la tuile.
 * @param start_index Index du joueur à supprimer.
 */
static void shift_players_array(tile_t *tile, int start_index)
{
    for (int j = start_index; j < tile->player_count - 1; j++) {
        tile->players[j] = tile->players[j + 1];
    }
}

/**
 * @brief Retire un joueur de la tuile où il se trouve actuellement.
 *
 * @param server Pointeur vers la structure serveur.
 * @param player Pointeur vers le joueur à retirer.
 */
static void remove_player_from_tile(server_t *server, player_t *player)
{
    tile_t *tile = game_get_tile(server->game, player->x, player->y);

    if (!tile)
        return;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i] == player) {
            shift_players_array(tile, i);
            tile->player_count--;
            break;
        }
    }
}

/**
 * @brief Détruit un joueur (libération mémoire + décrémentation équipe).
 *
 * @param player Pointeur vers le joueur à détruire.
 */
void player_destroy(player_t *player)
{
    if (!player)
        return;
    if (player->team)
        player->team->connected_clients--;
    printf("DEBUG: Destroyed player ID %d\n", player->id);
    free(player);
}

/**
 * @brief Gère la mort d’un joueur (envoie "dead", suppression de la tuile, GUI...).
 *
 * @param server Pointeur vers le serveur.
 * @param player Pointeur vers le joueur à tuer.
 */
void player_die(server_t *server, player_t *player)
{
    client_t *client = NULL;

    if (!player)
        return;
    printf("DEBUG: Player ID %d is dying\n", player->id);
    gui_broadcast_player_death(server, player);
    remove_player_from_tile(server, player);
    for (client = server->clients; client; client = client->next) {
        if (client->player == player) {
            network_send(client, "dead\n");
            client->player = NULL;
            break;
        }
    }
    player_destroy(player);
}

/**
 * @brief Fonction de mise à jour du joueur (placeholder pour extension future).
 *
 * @param server Pointeur vers le serveur.
 * @param player Pointeur vers le joueur à mettre à jour.
 */
void player_update(server_t *server, player_t *player)
{
    (void)server;
    (void)player;
}
