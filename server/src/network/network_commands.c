/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network commands processing
*/

#include "server.h"

typedef struct {
    const char *name;
    void (*func)(t_server *, t_client *, char **);
    int time;
} command_t;

static const command_t COMMANDS[] = {
    {"Forward", cmd_forward, 7},
    {"Right", cmd_right, 7},
    {"Left", cmd_left, 7},
    {"Look", cmd_look, 7},
    {"Inventory", cmd_inventory, 1},
    {"Broadcast", cmd_broadcast, 7},
    {"Connect_nbr", cmd_connect_nbr, 0},
    {"Fork", cmd_fork, 42},
    {"Eject", cmd_eject, 7},
    {"Take", cmd_take, 7},
    {"Set", cmd_set, 7},
    {"Incantation", cmd_incantation, 300},
    {NULL, NULL, 0}
};

static const command_t *find_command(const char *name)
{
    for (int i = 0; COMMANDS[i].name; i++) {
        if (strcmp(COMMANDS[i].name, name) == 0)
            return &COMMANDS[i];
    }
    return NULL;
}

static void execute_player_command(t_server *server, t_client *client,
    char **args)
{
    const command_t *cmd = NULL;

    if (!args || !args[0])
        return;
    cmd = find_command(args[0]);
    if (cmd) {
        client->player->action_time = cmd->time;
        cmd->func(server, client, args);
    } else {
        network_send(client, "ko\n");
    }
}

void network_process_commands(t_server *server, t_client *client)
{
    t_command *cmd = NULL;
    char **args = NULL;

    if (!client->cmd_queue || client->player->action_time > 0)
        return;
    cmd = client->cmd_queue;
    client->cmd_queue = cmd->next;
    client->cmd_count--;
    args = str_split(cmd->cmd, ' ');
    if (args && args[0])
        execute_player_command(server, client, args);
    else
        network_send(client, "ko\n");
    free_array(args);
    free(cmd->cmd);
    free(cmd);
}
