/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_look
*/

#include "server.h"

static bool validate_look_request(client_t *client)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

static void finalize_look_response(client_t *client, char *buffer)
{
    strcat(buffer, "]\n");
    network_send(client, buffer);
}

void cmd_look(server_t *server, client_t *client, char **args)
{
    char buffer[16384] = "[";

    (void)args;
    if (!validate_look_request(client))
        return;
    build_player_vision(server, client->player, buffer);
    finalize_look_response(client, buffer);
}
