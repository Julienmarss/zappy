/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network player connection handling - Part 2
*/

#include "server.h"

static player_t *setup_player_from_egg(server_t *server, team_t *team,
    egg_t *egg)
{
    player_t *player = player_create(team, egg->x, egg->y);

    if (!player)
        return NULL;
    team->connected_clients++;
    player_add_to_tile(server, player);
    return player;
}

static bool check_connection_validity(team_t *team, egg_t *egg)
{
    if (!egg || team->connected_clients >= team->max_clients)
        return false;
    return true;
}

static void setup_player_client(client_t *client, player_t *player)
{
    client->player = player;
    player->client = client;
    client->type = CLIENT_PLAYER;
}

static egg_t *get_available_egg_from_team(team_t *team)
{
    if (!team->eggs)
        return NULL;
    return team->eggs;
}

void network_reject_connection(server_t *server, client_t *client)
{
    network_send(client, "ko\n");
    server_disconnect_client(server, client);
}

void network_handle_player_connection(server_t *server, client_t *client,
    team_t *team)
{
    egg_t *egg = get_available_egg_from_team(team);
    player_t *player = NULL;

    if (!check_connection_validity(team, egg)) {
        network_reject_connection(server, client);
        return;
    }
    player = setup_player_from_egg(server, team, egg);
    if (!player) {
        network_reject_connection(server, client);
        return;
    }
    setup_player_client(client, player);
    network_send_connection_info(client, team, server->game->width,
        server->game->height);
}

void network_send_connection_info(client_t *client, team_t *team,
    int width, int height)
{
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "%d\n",
        team->max_clients - team->connected_clients);
    network_send(client, buffer);
    snprintf(buffer, sizeof(buffer), "%d %d\n", width, height);
    network_send(client, buffer);
}
