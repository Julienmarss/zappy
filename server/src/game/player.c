/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player implementation
*/

#include "server.h"

t_player *player_create(t_team *team, int x, int y)
{
    t_player *player = calloc(1, sizeof(t_player));
    static int id_counter = 0;

    if (!player)
        return NULL;
    player->id = ++id_counter;
    player->x = x;
    player->y = y;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    player->inventory[FOOD] = 10;
    player->food_timer = 126;
    player->team = team;
    player->is_incanting = false;
    player->action_time = 0;
    return player;
}

void player_destroy(t_player *player)
{
    if (!player)
        return;
    if (player->team)
        player->team->connected_clients--;
    free(player);
}

void player_die(t_server *server, t_player *player)
{
    t_client *client = NULL;

    if (!player)
        return;
    for (client = server->clients; client; client = client->next) {
        if (client->player == player) {
            network_send(client, "dead\n");
            client->player = NULL;
            break;
        }
    }
    player_destroy(player);
}

void player_update(t_server *server, t_player *player)
{
    (void)server;
    (void)player;
}
