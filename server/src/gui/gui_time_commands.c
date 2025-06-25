/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_time_commands
*/

#include "gui_protocol.h"

void gui_cmd_sgt(server_t *server, client_t *client, char **args)
{
    char response[MAX_GUI_RESPONSE];

    (void)args;
    if (!server || !server->game) {
        gui_send_bad_parameters(client);
        return;
    }
    snprintf(response, sizeof(response), "sgt %d\n", server->game->time_unit);
    network_send(client, response);
    printf("DEBUG: Sent time unit to GUI: %d\n", server->game->time_unit);
}

static bool validate_time_unit(int time_unit)
{
    if (time_unit < 2 || time_unit > 10000) {
        printf("DEBUG: Invalid time unit: %d (must be 2-10000)\n", time_unit);
        return false;
    }
    return true;
}

void gui_cmd_sst(server_t *server, client_t *client, char **args)
{
    int new_time_unit = 0;
    char response[MAX_GUI_RESPONSE];

    if (!args[1]) {
        gui_send_bad_parameters(client);
        return;
    }
    new_time_unit = atoi(args[1]);
    if (!validate_time_unit(new_time_unit)) {
        gui_send_bad_parameters(client);
        return;
    }
    printf("DEBUG: Changing time unit from %d to %d\n",
        server->game->time_unit, new_time_unit);
    server->game->time_unit = new_time_unit;
    snprintf(response, sizeof(response), "sst %d\n", new_time_unit);
    network_send(client, response);
    printf("DEBUG: Time unit changed successfully to %d\n", new_time_unit);
}
