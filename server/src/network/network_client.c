/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handling for network - Part 1
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
    return buffer;
}

static team_t *find_team(game_t *game, const char *name)
{
    team_t *team = game->teams;

    while (team) {
        if (strcmp(team->name, name) == 0)
            return team;
        team = team->next;
    }
    return NULL;
}

static egg_t *get_available_egg(team_t *team)
{
    if (!team->eggs)
        return NULL;
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
}

static void handle_team_connection(server_t *server, client_t *client,
    const char *clean_name)
{
    team_t *team = find_team(server->game, clean_name);

    if (!team) {
        network_reject_connection(server, client);
        return;
    }
    network_handle_player_connection(server, client, team);
}

void network_handle_new_client(server_t *server, client_t *client,
    const char *team_name)
{
    char *clean_name = clean_string_simple(team_name);

    if (strcmp(clean_name, "GRAPHIC") == 0) {
        network_handle_graphic_connection(server, client);
        return;
    }
    handle_team_connection(server, client, clean_name);
}
