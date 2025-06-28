/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_look
*/

#include "server.h"
#include "vision.h"

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
    strcat(buffer, "\n");
    network_send(client, buffer);
}

void cmd_look(server_t *server, client_t *client, char **args)
{
    char buffer[16384] = {0};
    vision_data_t *vision_data = NULL;

    (void)args;
    if (!validate_look_request(client))
        return;
    vision_data = vision_create_data(client->player->level);
    if (!vision_data) {
        network_send(client, "ko\n");
        return;
    }
    vision_calculate_tiles(vision_data);
    vision_format_response(server, client->player, vision_data, buffer);
    vision_destroy_data(vision_data);
    finalize_look_response(client, buffer);
}
