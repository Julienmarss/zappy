/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_action
*/

#include "gui_protocol.h"

static bool is_player_valid(player_t *player)
{
    return player != NULL;
}

static bool is_message_valid(const char *message)
{
    return message != NULL;
}

void gui_broadcast_player_expulsion(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pex #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_fork(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pfk #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_broadcast(server_t *server, player_t *player,
    const char *broadcast_message)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player) || !is_message_valid(broadcast_message)) {
        return;
    }
    snprintf(message, sizeof(message), "pbc #%d %s\n",
        player->id, broadcast_message);
    gui_send_to_all_graphic_clients(server, message);
}
