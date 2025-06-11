/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handling for network
*/

#include "server.h"

static t_team *find_team(t_game *game, const char *name)
{
    t_team *team = game->teams;

    while (team) {
        if (strcmp(team->name, name) == 0)
            return team;
        team = team->next;
    }
    return NULL;
}

static t_egg *get_available_egg(t_team *team)
{
    t_egg *egg = team->eggs;

    while (egg) {
        bool is_used = false;
        
        if (!is_used)
            return egg;
        egg = egg->next;
    }
    return NULL;
}

static void send_connection_info(t_client *client, t_team *team,
    int width, int height)
{
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "%d\n", 
        team->max_clients - team->connected_clients);
    network_send(client, buffer);
    snprintf(buffer, sizeof(buffer), "%d %d\n", width, height);
    network_send(client, buffer);
}

static t_player *setup_player_from_egg(t_team *team, t_egg *egg)
{
    t_player *player = player_create(team, egg->x, egg->y);

    if (!player)
        return NULL;
    team->connected_clients++;
    return player;
}

static void handle_player_connection(t_server *server, t_client *client,
    t_team *team)
{
    t_egg *egg = get_available_egg(team);
    t_player *player = NULL;

    if (!egg || team->connected_clients >= team->max_clients) {
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    player = setup_player_from_egg(team, egg);
    if (!player) {
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    client->player = player;
    player->client = client;
    client->type = CLIENT_PLAYER;
    send_connection_info(client, team, server->game->width,
        server->game->height);
}

static void send_all_tiles(t_server *server, t_client *client)
{
    for (int y = 0; y < server->game->height; y++) {
        for (int x = 0; x < server->game->width; x++) {
            gui_send_tile_content(server, client, x, y);
        }
    }
}

static void send_all_players(t_server *server, t_client *client)
{
    t_client *c = server->clients;

    while (c) {
        if (c->type == CLIENT_PLAYER && c->player)
            gui_send_player_position(server, client, c->player);
        c = c->next;
    }
}

static void handle_graphic_connection(t_server *server, t_client *client)
{
    client->type = CLIENT_GRAPHIC;
    gui_send_map_size(server, client);
    gui_send_teams(server, client);
    send_all_tiles(server, client);
    send_all_players(server, client);
}

void network_handle_new_client(t_server *server, t_client *client,
    const char *team_name)
{
    t_team *team = NULL;

    if (strcmp(team_name, "GRAPHIC") == 0) {
        handle_graphic_connection(server, client);
        return;
    }
    team = find_team(server->game, team_name);
    if (!team) {
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    handle_player_connection(server, client, team);
}
