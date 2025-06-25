/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_incantation
*/

#include "gui_protocol.h"

static void add_player_to_list(char *buffer, player_t *player, bool is_first)
{
    if (!is_first) {
        strcat(buffer, " ");
    }
    strcat(buffer, gui_format_player_id(player->id));
}

static void format_players_list(char *buffer, player_t **players,
    int nb_players)
{
    int i = 0;

    for (i = 0; i < nb_players; i++) {
        if (players[i]) {
            add_player_to_list(buffer, players[i], i == 0);
        }
    }
}

static void send_incantation_message(server_t *server,
    incantation_data_t *data, const char *players_list)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pic %d %d %d %s\n",
        data->x, data->y, data->level, players_list);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_incantation_start(server_t *server,
    incantation_data_t *data)
{
    char players_list[512] = "";

    format_players_list(players_list, data->players, data->nb_players);
    send_incantation_message(server, data, players_list);
}

void gui_broadcast_incantation_end(server_t *server, int x, int y, bool result)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pie %d %d %d\n",
        x, y, result ? 1 : 0);
    gui_send_to_all_graphic_clients(server, message);
}
