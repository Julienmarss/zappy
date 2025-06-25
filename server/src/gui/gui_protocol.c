/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_protocol
*/

#include "gui_protocol.h"

static const gui_handler_t GUI_HANDLERS[] = {
    {"msz", gui_cmd_msz, false},
    {"bct", gui_cmd_bct, true},
    {"mct", gui_cmd_mct, false},
    {"tna", gui_cmd_tna, false},
    {"ppo", gui_cmd_ppo, true},
    {"plv", gui_cmd_plv, true},
    {"pin", gui_cmd_pin, true},
    {"sgt", gui_cmd_sgt, false},
    {"sst", gui_cmd_sst, true},
    {NULL, NULL, false}
};

static const gui_handler_t *find_gui_handler(const char *command)
{
    int i = 0;

    if (!command) {
        return NULL;
    }
    for (i = 0; GUI_HANDLERS[i].command; i++) {
        if (strcmp(GUI_HANDLERS[i].command, command) == 0) {
            return &GUI_HANDLERS[i];
        }
    }
    return NULL;
}

static bool validate_gui_command(const gui_handler_t *handler, char **args)
{
    if (!handler->need_args) {
        return true;
    }
    if (!args || !args[1]) {
        printf("DEBUG: GUI command needs arguments but none provided\n");
        return false;
    }
    return true;
}

static void execute_gui_command(server_t *server, client_t *client,
    const gui_handler_t *handler, char **args)
{
    printf("DEBUG: Executing GUI command '%s'\n", handler->command);
    handler->handler(server, client, args);
}

static bool process_gui_args(char **args, client_t *client)
{
    if (!args || !args[0]) {
        gui_send_unknown_command(client);
        return false;
    }
    printf("DEBUG: GUI received command: '%s'\n", args[0]);
    return true;
}

static bool process_gui_handler(const gui_handler_t *handler, char **args,
    client_t *client)
{
    if (!handler) {
        printf("DEBUG: Unknown GUI command: '%s'\n", args[0]);
        gui_send_unknown_command(client);
        return false;
    }
    if (!validate_gui_command(handler, args)) {
        gui_send_bad_parameters(client);
        return false;
    }
    return true;
}

void gui_handle_command(server_t *server, client_t *client, const char *line)
{
    char **args = str_split(line, ' ');
    const gui_handler_t *handler = NULL;

    if (!process_gui_args(args, client)) {
        free_array(args);
        return;
    }
    handler = find_gui_handler(args[0]);
    if (!process_gui_handler(handler, args, client)) {
        free_array(args);
        return;
    }
    execute_gui_command(server, client, handler, args);
    free_array(args);
}

void gui_send_unknown_command(client_t *client)
{
    network_send(client, "suc\n");
}

void gui_send_bad_parameters(client_t *client)
{
    network_send(client, "sbp\n");
}
