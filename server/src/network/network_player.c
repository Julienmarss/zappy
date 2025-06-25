/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_player
*/

#include "server.h"
#include "gui_protocol.h"

static player_t *setup_player_from_egg(server_t *server, team_t *team,
    egg_t *egg)
{
    player_t *player = player_create(team, egg->x, egg->y);

    if (!player) {
        return NULL;
    }
    team->connected_clients++;
    player_add_to_tile(server, player);
    return player;
}

static bool check_connection_validity(team_t *team, egg_t *egg)
{
    if (!egg || team->connected_clients >= team->max_clients) {
        return false;
    }
    return true;
}

static void setup_player_client(client_t *client, player_t *player)
{
    client->player = player;
    player->client = client;
    client->type = CLIENT_PLAYER;
}

static void finalize_connection(server_t *server, client_t *client,
    egg_t *egg, player_t *player)
{
    network_send_connection_info(client, player->team, server->game->width,
        server->game->height);
    gui_broadcast_egg_connection(server, egg);
    gui_broadcast_new_player(server, player);
    cleanup_egg_resources(server, player->team, egg);
    printf("DEBUG: Player connected successfully: ID %d at (%d,%d)\n",
        player->id, player->x, player->y);
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
    finalize_connection(server, client, egg, player);
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
