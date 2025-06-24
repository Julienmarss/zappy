/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game end condition checking functions (1 public + 2 static)
*/

#include "incantation.h"
#include "gui_protocol.h"

static int count_level_eight_players(server_t *server, team_t *team)
{
    client_t *client = server->clients;
    int count = 0;

    while (client) {
        if (client->type == CLIENT_PLAYER && client->player &&
            client->player->team == team && client->player->level >= 8) {
            count++;
        }
        client = client->next;
    }
    return count;
}

static bool team_has_won(server_t *server, team_t *team)
{
    int level_eight_count = count_level_eight_players(server, team);

    return level_eight_count >= 6;
}

void check_game_end_condition(server_t *server)
{
    team_t *team = server->game->teams;

    while (team) {
        if (team_has_won(server, team)) {
            gui_broadcast_game_end(server, team->name);
            return;
        }
        team = team->next;
    }
}
