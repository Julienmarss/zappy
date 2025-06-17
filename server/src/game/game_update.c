/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game update logic - FIXED HUNGER SYSTEM
*/

#include "server.h"

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

static void consume_food_if_available(player_t *player)
{
    if (player->inventory[FOOD] > 0) {
        player->inventory[FOOD]--;
        player->food_timer = 126;
        printf("DEBUG: Player ID %d consumed food, remaining: %d\n",
            player->id, player->inventory[FOOD]);
    } else {
        printf("DEBUG: Player ID %d starved to death\n", player->id);
    }
}

static void update_player_hunger(server_t *server, player_t *player)
{
    player->food_timer--;
    printf("DEBUG: Player ID %d food timer: %d\n", player->id,
        player->food_timer);
    if (player->food_timer <= 0) {
        if (player->inventory[FOOD] > 0)
            consume_food_if_available(player);
        else
            player_die(server, player);
    }
}

static void update_player_food(server_t *server)
{
    client_t *client = server->clients;
    static int food_tick = 0;

    food_tick++;
    if (food_tick < 126)
        return;
    food_tick = 0;
    while (client) {
        if (client->type == CLIENT_PLAYER && client->player)
            update_player_hunger(server, client->player);
        client = client->next;
    }
}

void game_update(server_t *server)
{
    static int tick = 0;

    update_player_actions(server);
    update_player_food(server);
    tick++;
    if (tick >= 20) {
        game_spawn_resources(server->game);
        tick = 0;
    }
}
