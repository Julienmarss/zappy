/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_resource
*/

#include "gui_protocol.h"

void gui_broadcast_resource_drop(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        return;
    }
    snprintf(message, sizeof(message), "pdr #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_resource_collect(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        return;
    }
    snprintf(message, sizeof(message), "pgt #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
}
