/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game update logic
*/

#include "server.h"

static void update_player_actions(t_server *server)
{
    t_client *client = server->clients;
    t_player *player = NULL;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player) {
            player = client->player;
            if (player->action_time > 0) {
                player->action_time--;
            } else if (client->cmd_queue &&
                client->cmd_queue->time_limit == 0) {
                network_process_commands(server, client);
            }
        }
        client = client->next;
    }
}

static void update_player_food(t_server *server)
{
    t_client *client = server->clients;
    t_player *player = NULL;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player) {
            player = client->player;
            player->food_timer--;
            if (player->food_timer <= 0) {
                if (player->inventory[FOOD] > 0) {
                    player->inventory[FOOD]--;
                    player->food_timer = 126;
                } else {
                    player_die(server, player);
                }
            }
        }
        client = client->next;
    }
}

void game_update(t_server *server)
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
