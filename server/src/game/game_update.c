/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_update
*/

/**
 * @file game_update.c
 * @brief Contient les fonctions liées à la mise à jour périodique du jeu, comme les actions des joueurs, la nourriture et le spawn de ressources.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Diminue le compteur de temps d'action du joueur.
 *
 * Lorsque le temps atteint 0, l'action peut être traitée.
 *
 * @param player Le joueur concerné.
 */
static void handle_player_action_time(player_t *player)
{
    if (player->action_time > 0) {
        player->action_time--;
        if (player->action_time == 0)
            printf("DEBUG: Player action completed\n");
    }
}

/**
 * @brief Traite les commandes du joueur prêtes à être exécutées.
 *
 * @param server Le serveur du jeu.
 * @param client Le client du joueur.
 */
static void process_player_commands(server_t *server, client_t *client)
{
    if (client->cmd_queue && client->cmd_queue->time_limit == 0)
        network_process_commands(server, client);
}

/**
 * @brief Met à jour toutes les actions des joueurs connectés.
 *
 * Parcourt la liste des clients et met à jour les timers et les actions des joueurs.
 *
 * @param server Le serveur du jeu.
 */
static void update_player_actions(server_t *server)
{
    client_t *client = server->clients;
    player_t *player = NULL;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player) {
            player = client->player;
            handle_player_action_time(player);
            process_player_commands(server, client);
        }
        client = client->next;
    }
}

/**
 * @brief Fait consommer de la nourriture au joueur si disponible.
 *
 * Réinitialise le timer de nourriture ou tue le joueur s'il n'a plus de nourriture.
 *
 * @param server Le serveur du jeu.
 * @param player Le joueur concerné.
 */
static void consume_food_if_available(server_t *server, player_t *player)
{
    if (player->inventory[FOOD] > 0) {
        player->inventory[FOOD]--;
        player->food_timer = 126;
        gui_broadcast_player_inventory(server, player);
    } else {
        printf("DEBUG: Player ID %d starved to death\n", player->id);
    }
}

/**
 * @brief Met à jour la gestion de la faim pour un joueur spécifique.
 *
 * Diminue le timer de nourriture et appelle la fonction de consommation ou de mort.
 *
 * @param server Le serveur du jeu.
 * @param player Le joueur concerné.
 */
static void update_single_player_food(server_t *server, player_t *player)
{
    player->food_timer--;
    printf("DEBUG: Player ID %d food timer: %d\n", player->id,
        player->food_timer);
    if (player->food_timer <= 0) {
        if (player->inventory[FOOD] > 0)
            consume_food_if_available(server, player);
        else
            player_die(server, player);
    }
}

/**
 * @brief Met à jour la nourriture de tous les joueurs connectés.
 *
 * @param server Le serveur du jeu.
 */
static void update_player_food(server_t *server)
{
    client_t *client = server->clients;
    client_t *next_client = NULL;

    while (client) {
        next_client = client->next;
        if (client->type == CLIENT_PLAYER && client->player)
            update_single_player_food(server, client->player);
        client = next_client;
    }
}

/**
 * @brief Fonction principale appelée à chaque tick pour mettre à jour le jeu.
 *
 * Cette fonction :
 * - met à jour les actions des joueurs,
 * - met à jour la gestion de la nourriture,
 * - déclenche le spawn périodique de ressources toutes les 20 itérations.
 *
 * @param server Le serveur du jeu.
 */
void game_update(server_t *server)
{
    static int resource_spawn_tick = 0;

    update_player_actions(server);
    update_player_food(server);
    resource_spawn_tick++;
    if (resource_spawn_tick >= 20) {
        game_spawn_resources(server);
        resource_spawn_tick = 0;
    }
}
