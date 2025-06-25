/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_game
*/

#include "gui_protocol.h"

void gui_broadcast_game_end(server_t *server, const char *team_name)
{
    char message[MAX_GUI_RESPONSE];

    if (!team_name) {
        return;
    }
    snprintf(message, sizeof(message), "seg %s\n", team_name);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_server_message(server_t *server, const char *server_msg)
{
    char message[MAX_GUI_RESPONSE];

    if (!server_msg) {
        return;
    }
    snprintf(message, sizeof(message), "smg %s\n", server_msg);
    gui_send_to_all_graphic_clients(server, message);
}
