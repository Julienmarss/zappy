/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI broadcast functions for server events
*/

#include "gui.h"

void gui_broadcast_new_player(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || !player->team) {
        printf("DEBUG: Cannot broadcast new player - invalid data\n");
        return;
    }
    snprintf(message, sizeof(message), "pnw #%d %d %d %d %d %s\n",
        player->id, player->x, player->y, player->orientation,
        player->level, player->team->name);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted new player connection: ID %d\n", player->id);
}

void gui_broadcast_player_position(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player position - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message), "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->orientation);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_level(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player level - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message), "plv #%d %d\n",
        player->id, player->level);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_inventory(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player inventory - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD],
        player->inventory[LINEMATE],
        player->inventory[DERAUMERE],
        player->inventory[SIBUR],
        player->inventory[MENDIANE],
        player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
    gui_send_to_all_graphic_clients(server, message);
}

void gui_broadcast_player_expulsion(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player expulsion - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message), "pex #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted player expulsion: ID %d\n", player->id);
}

void gui_broadcast_player_broadcast(server_t *server, player_t *player,
    const char *message)
{
    char response[MAX_GUI_RESPONSE];

    if (!player || !message) {
        printf("DEBUG: Cannot broadcast player message - invalid data\n");
        return;
    }
    snprintf(response, sizeof(response), "pbc #%d %s\n",
        player->id, message);
    gui_send_to_all_graphic_clients(server, response);
    printf("DEBUG: Broadcasted player broadcast: ID %d\n", player->id);
}

static void format_players_list(char *buffer, player_t **players,
    int nb_players)
{
    int i = 0;

    for (i = 0; i < nb_players; i++) {
        if (players[i]) {
            if (i > 0)
                strcat(buffer, " ");
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
    printf("DEBUG: Broadcasted incantation start at (%d,%d) level %d\n",
        x, y, level);
}

void gui_broadcast_incantation_end(server_t *server, int x, int y, bool result)
{
    char message[MAX_GUI_RESPONSE];

    snprintf(message, sizeof(message), "pie %d %d %d\n",
        x, y, result ? 1 : 0);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted incantation end at (%d,%d) result: %s\n",
        x, y, result ? "success" : "failure");
}

void gui_broadcast_player_fork(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player fork - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message), "pfk #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted player fork: ID %d\n", player->id);
}

void gui_broadcast_resource_drop(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        printf("DEBUG: Cannot broadcast resource drop - invalid data\n");
        return;
    }
    snprintf(message, sizeof(message), "pdr #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted resource drop: player %d, resource %d\n",
        player->id, resource);
}

void gui_broadcast_resource_collect(server_t *server, player_t *player,
    int resource)
{
    char message[MAX_GUI_RESPONSE];

    if (!player || resource < 0 || resource >= NB_RESOURCES) {
        printf("DEBUG: Cannot broadcast resource collect - invalid data\n");
        return;
    }
    snprintf(message, sizeof(message), "pgt #%d %d\n",
        player->id, resource);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted resource collect: player %d, resource %d\n",
        player->id, resource);
}

void gui_broadcast_player_death(server_t *server, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!player) {
        printf("DEBUG: Cannot broadcast player death - invalid player\n");
        return;
    }
    snprintf(message, sizeof(message), "pdi #%d\n", player->id);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted player death: ID %d\n", player->id);
}

void gui_broadcast_egg_laid(server_t *server, egg_t *egg, player_t *player)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg || !player) {
        printf("DEBUG: Cannot broadcast egg laid - invalid data\n");
        return;
    }
    snprintf(message, sizeof(message), "enw #%d #%d %d %d\n",
        egg->id, player->id, egg->x, egg->y);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted egg laid: egg %d by player %d at (%d,%d)\n",
        egg->id, player->id, egg->x, egg->y);
}

void gui_broadcast_egg_connection(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        printf("DEBUG: Cannot broadcast egg connection - invalid egg\n");
        return;
    }
    snprintf(message, sizeof(message), "ebo #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted egg connection: ID %d\n", egg->id);
}

void gui_broadcast_egg_death(server_t *server, egg_t *egg)
{
    char message[MAX_GUI_RESPONSE];

    if (!egg) {
        printf("DEBUG: Cannot broadcast egg death - invalid egg\n");
        return;
    }
    snprintf(message, sizeof(message), "edi #%d\n", egg->id);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted egg death: ID %d\n", egg->id);
}

void gui_broadcast_game_end(server_t *server, const char *team_name)
{
    char message[MAX_GUI_RESPONSE];

    if (!team_name) {
        printf("DEBUG: Cannot broadcast game end - invalid team name\n");
        return;
    }
    snprintf(message, sizeof(message), "seg %s\n", team_name);
    gui_send_to_all_graphic_clients(server, message);
    printf("DEBUG: Broadcasted game end: winning team %s\n", team_name);
}

void gui_broadcast_server_message(server_t *server, const char *message)
{
    char response[MAX_GUI_RESPONSE];

    if (!message) {
        printf("DEBUG: Cannot broadcast server message - invalid message\n");
        return;
    }
    snprintf(response, sizeof(response), "smg %s\n", message);
    gui_send_to_all_graphic_clients(server, response);
    printf("DEBUG: Broadcasted server message: %s\n", message);
}
