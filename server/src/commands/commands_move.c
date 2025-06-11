/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Movement commands implementation
*/

#include "server.h"

static void move_player_forward(t_player *player, t_game *game)
{
    int dx = 0;
    int dy = 0;

    if (player->orientation == NORTH)
        dy = -1;
    else if (player->orientation == SOUTH)
        dy = 1;
    else if (player->orientation == EAST)
        dx = 1;
    else if (player->orientation == WEST)
        dx = -1;
    player->x = (player->x + dx + game->width) % game->width;
    player->y = (player->y + dy + game->height) % game->height;
}

void cmd_forward(t_server *server, t_client *client, char **args)
{
    t_player *player = client->player;

    (void)args;
    if (!player)
        return;
    move_player_forward(player, server->game);
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "move");
}

void cmd_right(t_server *server, t_client *client, char **args)
{
    t_player *player = client->player;

    (void)args;
    if (!player)
        return;
    player->orientation = (player->orientation % 4) + 1;
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "turn");
}

void cmd_left(t_server *server, t_client *client, char **args)
{
    t_player *player = client->player;

    (void)args;
    if (!player)
        return;
    player->orientation = player->orientation - 1;
    if (player->orientation < 1)
        player->orientation = 4;
    network_send(client, "ok\n");
    gui_broadcast_player_action(server, player, "turn");
}
