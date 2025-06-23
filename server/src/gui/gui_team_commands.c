/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI team commands implementation
*/

#include "gui_protocol.h"

static void send_single_team_name(client_t *client, const char *team_name)
{
    char response[MAX_GUI_RESPONSE];

    snprintf(response, sizeof(response), "tna %s\n", team_name);
    network_send(client, response);
    printf("DEBUG: Sent team name: %s\n", team_name);
}

void gui_cmd_tna(server_t *server, client_t *client, char **args)
{
    team_t *team = NULL;
    int team_count = 0;

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Sending all team names to GUI\n");
    team = server->game->teams;
    while (team) {
        send_single_team_name(client, team->name);
        team_count++;
        team = team->next;
    }
    printf("DEBUG: Sent %d team names\n", team_count);
}
