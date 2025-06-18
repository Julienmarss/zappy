/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Look command implementation - main function
*/

#include "server.h"

static bool validate_look_request(client_t *client)
{
    if (!client->player) {
        printf("DEBUG: Look - No player\n");
        network_send(client, "ko\n");
        return false;
    }
    printf("DEBUG: Look - Player ID %d at (%d,%d), level %d, orient %d\n",
        client->player->id, client->player->x, client->player->y,
        client->player->level, client->player->orientation);
    return true;
}

static void finalize_look_response(client_t *client, char *buffer)
{
    strcat(buffer, "]\n");
    printf("DEBUG: Look result (level %d): %s", client->player->level,
        buffer);
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
