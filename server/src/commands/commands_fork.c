/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_fork
*/

#include "server.h"
#include "gui_protocol.h"

static egg_t *create_new_egg(server_t *server, player_t *player)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg) {
        return NULL;
    }
    memset(egg, 0, sizeof(egg_t));
    egg->id = server->game->egg_id_counter;
    server->game->egg_id_counter++;
    egg->x = player->x;
    egg->y = player->y;
    egg->team = player->team;
    egg->next = NULL;
    printf("DEBUG: Created egg ID %d at (%d,%d) for team %s\n",
        egg->id, egg->x, egg->y, player->team->name);
    return egg;
}

static void add_egg_to_team(team_t *team, egg_t *egg)
{
    egg->next = team->eggs;
    team->eggs = egg;
    printf("DEBUG: Successfully added egg to team %s\n", team->name);
}

static void add_egg_to_tile(server_t *server, egg_t *egg)
{
    tile_t *tile = game_get_tile(server->game, egg->x, egg->y);

    if (!tile) {
        printf("DEBUG: Failed to get tile at (%d,%d)\n", egg->x, egg->y);
        return;
    }
    printf("DEBUG: Starting to add egg to tile (%d,%d)\n", egg->x, egg->y);
    if (tile->eggs) {
        egg->next = tile->eggs;
    } else {
        egg->next = NULL;
    }
    tile->eggs = egg;
    printf("DEBUG: Added egg ID %d to tile (%d,%d)\n",
        egg->id, egg->x, egg->y);
}

static bool validate_fork_prerequisites(client_t *client)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    if (!client->player->team) {
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

static void handle_fork_success(server_t *server, client_t *client,
    egg_t *egg)
{
    network_send(client, "ok\n");
    gui_broadcast_player_fork(server, client->player);
    gui_broadcast_egg_laid(server, egg, client->player);
}

void cmd_fork(server_t *server, client_t *client, char **args)
{
    egg_t *egg = NULL;

    (void)args;
    if (!validate_fork_prerequisites(client)) {
        return;
    }
    egg = create_new_egg(server, client->player);
    if (!egg) {
        network_send(client, "ko\n");
        return;
    }
    add_egg_to_team(client->player->team, egg);
    add_egg_to_tile(server, egg);
    handle_fork_success(server, client, egg);
}
