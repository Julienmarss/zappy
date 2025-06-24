/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI broadcast functions for incantation events (2 public + 1 static)
*/

#include "gui_protocol.h"

static void format_players_list(char *buffer, player_t **players,
    int nb_players)
{
    int i = 0;

    for (i = 0; i < nb_players; i++) {
        if (players[i]) {
            if (i > 0) {
                strcat(buffer, " ");
            }
            strcat(buffer, gui_format_player_id(players[i]->id));
        }
    }
}

void gui_broadcast_incantation_start(server_t *server, int x, int y, int level,
    player_t **players, int nb_players)
{
    char message[MAX_GUI_RESPONSE];
    char players_list[512] = "";

    format_players_list(players_list, players, nb_players);
    snprintf(message, sizeof(message), "pic %d %d %d %s\n",
        x, y, level, players_list);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_incantation_end(server_t *server, int x, int y, bool result)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pie %d %d %d\n",
        x, y, result ? 1 : 0);
    gui_send_to_all_graphic_clients(server, message);
}
