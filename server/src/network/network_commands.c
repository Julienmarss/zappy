/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network_commands
*/

#include "server.h"

static const cmd_info_t COMMANDS[] = {
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

static const cmd_info_t *find_command(const char *name)
{
    printf("DEBUG: Looking for command '%s'\n", name);
    for (int i = 0; COMMANDS[i].name; i++) {
        printf("DEBUG: Comparing with '%s'\n", COMMANDS[i].name);
        if (strcmp(COMMANDS[i].name, name) == 0) {
            printf("DEBUG: Command found! Time: %d\n", COMMANDS[i].time);
            return &COMMANDS[i];
        }
    }
    printf("DEBUG: Command not found\n");
    return NULL;
}

static void execute_player_command(server_t *server, client_t *client,
    char **args)
{
    const cmd_info_t *cmd = NULL;

    if (!args || !args[0]) {
        printf("DEBUG: No command args\n");
        return;
    }
    printf("DEBUG: Executing command '%s'\n", args[0]);
    cmd = find_command(args[0]);
    if (cmd) {
        client->player->action_time = cmd->time;
        printf("DEBUG: Command '%s' will take %d time units\n",
            args[0], cmd->time);
        cmd->func(server, client, args);
    } else {
        printf("DEBUG: Unknown command '%s', sending ko\n", args[0]);
        network_send(client, "ko\n");
    }
}

void network_process_commands(server_t *server, client_t *client)
{
    command_t *cmd = NULL;
    char **args = NULL;

    if (!client->cmd_queue || client->player->action_time > 0) {
        printf("DEBUG: No commands to process or player busy "
            "(action_time: %d)\n",
            client->player ? client->player->action_time : -1);
        return;
    }
    cmd = client->cmd_queue;
    client->cmd_queue = cmd->next;
    client->cmd_count--;
    printf("DEBUG: Processing command: '%s'\n", cmd->cmd);
    args = str_split(cmd->cmd, ' ');
    if (args && args[0])
        execute_player_command(server, client, args);
    else
        network_send(client, "ko\n");
    free_array(args);
    free(cmd->cmd);
}
