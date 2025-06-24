/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_incantation
*/

#include "server.h"
#include "gui_protocol.h"
#include "incantation.h"

void cmd_incantation(server_t *server, client_t *client, char **args)
{
    player_t *player = client->player;
    incantation_ctx_t ctx = {0};

    (void)args;
    if (!validate_incantation_player(client)) {
        return;
    }
    if (!check_incantation_conditions(server, player, &ctx)) {
        return;
    }
    execute_incantation_ritual(server, player, &ctx);
}
