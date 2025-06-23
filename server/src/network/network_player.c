/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_player
*/

#include "server.h"
#include "gui.h"

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

static void remove_egg_from_team(team_t *team, egg_t *egg)
{
    egg_t **current = &team->eggs;

    while (*current) {
        if (*current == egg) {
            *current = egg->next;
            break;
        }
        current = &(*current)->next;
    }
}

static void remove_egg_from_tile(server_t *server, egg_t *egg)
{
    tile_t *tile = game_get_tile(server->game, egg->x, egg->y);
    egg_t **current = NULL;

    if (!tile)
        return;
    current = &tile->eggs;
    while (*current) {
        if (*current == egg) {
            *current = egg->next;
            break;
        }
        current = &(*current)->next;
    }
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
    gui_broadcast_egg_connection(server, egg);
    gui_broadcast_new_player(server, player);
    remove_egg_from_team(team, egg);
    remove_egg_from_tile(server, egg);
    free(egg);
    printf("DEBUG: Player connected successfully: ID %d at (%d,%d)\n",
        player->id, player->x, player->y);
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
