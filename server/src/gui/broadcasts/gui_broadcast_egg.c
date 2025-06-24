/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI broadcast functions for egg events (3 public + 0 static)
*/

#include "gui_protocol.h"

void gui_broadcast_egg_laid(server_t *server, egg_t *egg, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg || !player) {
        return;
    }
    snprintf(message, sizeof(message), "enw #%d #%d %d %d\n",
        egg->id, player->id, egg->x, egg->y);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_egg_connection(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        return;
    }
    snprintf(message, sizeof(message), "ebo #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_egg_death(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        return;
    }
    snprintf(message, sizeof(message), "edi #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
}
