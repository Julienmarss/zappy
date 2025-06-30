/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_update
*/

#include "server.h"
#include "gui_protocol.h"

static void handle_player_action_time(player_t *player)
{
    if (player->action_time > 0) {
        player->action_time--;
        if (player->action_time == 0)
            printf("DEBUG: Player action completed\n");
    }
}

static void process_player_commands(server_t *server, client_t *client)
{
    if (client->cmd_queue && client->cmd_queue->time_limit == 0)
        network_process_commands(server, client);
}

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
