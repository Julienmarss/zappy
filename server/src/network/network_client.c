/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handling for network - SIMPLE STRING FIX
*/

#include "server.h"

static char *clean_string_simple(const char *str)
{
    static char buffer[256];
    int len = 0;
    int i = 0;

    if (!str)
        return NULL;
    
    memset(buffer, 0, sizeof(buffer));
    len = strlen(str);
    
    for (i = 0; i < len && i < 255; i++) {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != '\0') {
            buffer[i] = str[i];
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    
    printf("DEBUG: Cleaned '%s' -> '%s' (len: %d)\n", str, buffer, (int)strlen(buffer));
    return buffer;
}

static team_t *find_team(game_t *game, const char *name)
{
    team_t *team = game->teams;

    printf("DEBUG: Looking for team '%s'\n", name);
    while (team) {
        printf("DEBUG: Comparing with team '%s'\n", team->name);
        if (strcmp(team->name, name) == 0) {
            printf("DEBUG: Team found!\n");
            return team;
        }
        team = team->next;
    }
    printf("DEBUG: Team not found\n");
    return NULL;
}

static egg_t *get_available_egg(team_t *team)
{
    printf("DEBUG: Looking for available egg in team %s\n", team->name);
    printf("DEBUG: Team eggs pointer: %p\n", (void*)team->eggs);
    
    if (!team->eggs) {
        printf("DEBUG: No eggs in team\n");
        return NULL;
    }
    
    printf("DEBUG: Found egg ID %d at (%d,%d)\n", 
           team->eggs->id, team->eggs->x, team->eggs->y);
    return team->eggs;
}

static void send_connection_info(client_t *client, team_t *team,
    int width, int height)
{
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "%d\n", 
        team->max_clients - team->connected_clients);
    network_send(client, buffer);
    snprintf(buffer, sizeof(buffer), "%d %d\n", width, height);
    network_send(client, buffer);
    printf("DEBUG: Sent connection info: slots=%d, map=%dx%d\n",
           team->max_clients - team->connected_clients, width, height);
}

static player_t *setup_player_from_egg(server_t *server, team_t *team, egg_t *egg)
{
    player_t *player = player_create(team, egg->x, egg->y);

    if (!player)
        return NULL;
    team->connected_clients++;
    player_add_to_tile(server, player);
    printf("DEBUG: Player created at (%d,%d), team clients: %d/%d\n",
           egg->x, egg->y, team->connected_clients, team->max_clients);
    return player;
}

static void handle_player_connection(server_t *server, client_t *client,
    team_t *team)
{
    egg_t *egg = get_available_egg(team);
    player_t *player = NULL;

    printf("=== PLAYER CONNECTION DEBUG ===\n");
    printf("Team: %s\n", team->name);
    printf("Team eggs: %p\n", (void*)team->eggs);
    printf("Connected clients: %d/%d\n", team->connected_clients, team->max_clients);
    
    if (team->eggs) {
        printf("First egg ID: %d at (%d,%d)\n", team->eggs->id, team->eggs->x, team->eggs->y);
    } else {
        printf("NO EGGS FOUND!\n");
    }
    printf("Available egg: %p\n", (void*)egg);
    printf("===============================\n");

    if (!egg || team->connected_clients >= team->max_clients) {
        printf("REJECTION: egg=%p, clients=%d/%d\n", (void*)egg, 
               team->connected_clients, team->max_clients);
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    
    player = setup_player_from_egg(server, team, egg);
    if (!player) {
        printf("FAILED TO CREATE PLAYER\n");
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    
    printf("PLAYER CREATED: ID=%d at (%d,%d)\n", player->id, player->x, player->y);
    client->player = player;
    player->client = client;
    client->type = CLIENT_PLAYER;
    send_connection_info(client, team, server->game->width,
        server->game->height);
}

static void send_all_tiles(server_t *server, client_t *client)
{
    for (int y = 0; y < server->game->height; y++) {
        for (int x = 0; x < server->game->width; x++) {
            gui_send_tile_content(server, client, x, y);
        }
    }
}

static void send_all_players(server_t *server, client_t *client)
{
    client_t *c = server->clients;

    while (c) {
        if (c->type == CLIENT_PLAYER && c->player)
            gui_send_player_position(server, client, c->player);
        c = c->next;
    }
}

static void handle_graphic_connection(server_t *server, client_t *client)
{
    client->type = CLIENT_GRAPHIC;
    gui_send_map_size(server, client);
    gui_send_teams(server, client);
    send_all_tiles(server, client);
    send_all_players(server, client);
}

void network_handle_new_client(server_t *server, client_t *client,
    const char *team_name)
{
    team_t *team = NULL;
    char *clean_name = clean_string_simple(team_name);
    
    printf("DEBUG: New client wants to join team '%s'\n", clean_name);
    
    if (strcmp(clean_name, "GRAPHIC") == 0) {
        printf("DEBUG: Graphic client connecting\n");
        handle_graphic_connection(server, client);
        return;
    }
    
    team = find_team(server->game, clean_name);
    if (!team) {
        printf("DEBUG: Team '%s' not found\n", clean_name);
        network_send(client, "ko\n");
        server_disconnect_client(server, client);
        return;
    }
    
    printf("DEBUG: Found team '%s'\n", team->name);
    handle_player_connection(server, client, team);
}
