/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_handler
*/

#include "server.h"
#include "gui_protocol.h"

static command_t *create_command(const char *line)
{
    command_t *cmd = calloc(1, sizeof(command_t));

    if (!cmd)
        return NULL;
    cmd->cmd = strdup(line);
    if (!cmd->cmd) {
        free(cmd);
        return NULL;
    }
    str_trim(cmd->cmd);
    cmd->time_limit = 0;
    cmd->next = NULL;
    return cmd;
}

static void add_command_to_queue(client_t *client, command_t *cmd)
{
    command_t *last = client->cmd_queue;

    if (!client->cmd_queue) {
        client->cmd_queue = cmd;
    } else {
        while (last->next)
            last = last->next;
        last->next = cmd;
    }
    client->cmd_count++;
}

static void handle_unknown_client(server_t *server, client_t *client,
    const char *line)
{
    network_handle_new_client(server, client, line);
}

static void handle_player_client(client_t *client, const char *line)
{
    command_t *cmd = NULL;

    if (client->cmd_count < MAX_CMD_QUEUE) {
        cmd = create_command(line);
        if (cmd)
            add_command_to_queue(client, cmd);
    }
}

static void handle_graphic_client(server_t *server, client_t *client,
    const char *line)
{
    printf("DEBUG: GUI client sent command: '%s'\n", line);
    gui_handle_command(server, client, line);
}

void network_handle_client_line(server_t *server, client_t *client,
    const char *line)
{
    if (client->type == CLIENT_UNKNOWN) {
        handle_unknown_client(server, client, line);
        return;
    }
    if (client->type == CLIENT_PLAYER) {
        handle_player_client(client, line);
        return;
    }
    if (client->type == CLIENT_GRAPHIC) {
        handle_graphic_client(server, client, line);
        return;
    }
    printf("DEBUG: Unknown client type: %d\n", client->type);
}
