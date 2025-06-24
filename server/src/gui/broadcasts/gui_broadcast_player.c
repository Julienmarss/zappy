/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_broadcast_player
*/

#include "gui_protocol.h"

static bool is_player_valid(player_t *player)
{
    return player != NULL;
}

static bool is_player_with_team_valid(player_t *player)
{
    return player != NULL && player->team != NULL;
}

static void format_new_player_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE, "pnw #%d %d %d %d %d %s\n",
        player->id, player->x, player->y, player->orientation,
        player->level, player->team->name);
}

static void format_player_position_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE, "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
}

static void format_player_inventory_message(char *message, player_t *player)
{
    snprintf(message, MAX_GUI_RESPONSE,
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
}

void gui_broadcast_new_player(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_with_team_valid(player)) {
        return;
    }
    format_new_player_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_position(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    format_player_position_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_level(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "plv #%d %d\n",
        player->id, player->level);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_inventory(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    format_player_inventory_message(message, player);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_death(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!is_player_valid(player)) {
        return;
    }
    snprintf(message, sizeof(message), "pdi #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
}
