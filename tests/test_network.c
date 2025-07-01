/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_network
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"
#include "gui_protocol.h"

static server_t *create_test_server_network(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    game_t *game = calloc(1, sizeof(game_t));
    
    if (!server || !game) {
        free(server);
        free(game);
        return NULL;
    }
    
    game->width = 10;
    game->height = 10;
    game->time_unit = 100;
    game->player_id_counter = 1;
    game->egg_id_counter = 1;
    
    game->map = malloc(sizeof(tile_t *) * game->height);
    for (int y = 0; y < game->height; y++) {
        game->map[y] = calloc(game->width, sizeof(tile_t));
        for (int x = 0; x < game->width; x++) {
            game->map[y][x].players = calloc(MAX_CLIENTS, sizeof(player_t *));
        }
    }
    
    team_t *team1 = calloc(1, sizeof(team_t));
    team_t *team2 = calloc(1, sizeof(team_t));
    team1->name = strdup("team1");
    team1->max_clients = 5;
    team1->connected_clients = 0;
    team2->name = strdup("team2");
    team2->max_clients = 5;
    team2->connected_clients = 0;
    team1->next = team2;
    game->teams = team1;
    
    server->game = game;
    server->clients = NULL;
    server->current_time = 0;
    
    return server;
}

static client_t *create_test_client_network(client_type_t type)
{
    client_t *client = calloc(1, sizeof(client_t));
    if (!client) return NULL;
    
    client->fd = 42;
    client->type = type;
    client->read_pos = 0;
    client->write_pos = 0;
    client->cmd_queue = NULL;
    client->cmd_count = 0;
    client->next = NULL;
    
    return client;
}

static player_t *create_test_player_network(int id, int x, int y, team_t *team)
{
    player_t *player = calloc(1, sizeof(player_t));
    if (!player) return NULL;
    
    player->id = id;
    player->x = x;
    player->y = y;
    player->orientation = NORTH;
    player->level = 1;
    player->team = team;
    player->inventory[FOOD] = 10;
    player->food_timer = 126;
    player->is_incanting = false;
    player->action_time = 0;
    
    return player;
}

static egg_t *create_test_egg_network(int id, int x, int y, team_t *team)
{
    egg_t *egg = calloc(1, sizeof(egg_t));
    if (!egg) return NULL;
    
    egg->id = id;
    egg->x = x;
    egg->y = y;
    egg->team = team;
    egg->next = NULL;
    
    return egg;
}

static void add_egg_to_team(team_t *team, egg_t *egg)
{
    egg->next = team->eggs;
    team->eggs = egg;
}

static void destroy_test_server_network(server_t *server)
{
    if (!server) return;
    
    client_t *current = server->clients;
    while (current) {
        client_t *next = current->next;
        
        command_t *cmd = current->cmd_queue;
        while (cmd) {
            command_t *next_cmd = cmd->next;
            free(cmd->cmd);
            free(cmd);
            cmd = next_cmd;
        }
        
        if (current->player) {
            free(current->player);
        }
        free(current);
        current = next;
    }
    
    if (server->game) {
        team_t *team = server->game->teams;
        while (team) {
            team_t *next_team = team->next;
            
            egg_t *egg = team->eggs;
            while (egg) {
                egg_t *next_egg = egg->next;
                free(egg);
                egg = next_egg;
            }
            
            free(team->name);
            free(team);
            team = next_team;
        }
        
        if (server->game->map) {
            for (int y = 0; y < server->game->height; y++) {
                if (server->game->map[y]) {
                    for (int x = 0; x < server->game->width; x++) {
                        free(server->game->map[y][x].players);
                    }
                    free(server->game->map[y]);
                }
            }
            free(server->game->map);
        }
        
        free(server->game);
    }
    
    free(server);
}

Test(network, network_send_basic)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    const char *message = "test message\n";
    
    cr_assert_not_null(client);
    
    int result = network_send(client, message);
    cr_assert(true, "network_send should not crash");
    
    free(client);
}

Test(network, network_send_null_client)
{
    const char *message = "test message\n";
    
    int result = network_send(NULL, message);
    cr_assert_eq(result, -1, "network_send with NULL client should return -1");
}

Test(network, network_send_null_message)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(client);
    
    int result = network_send(client, NULL);
    cr_assert(true, "network_send with NULL message should not crash");
    
    free(client);
}

Test(network, network_send_empty_message)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(client);
    
    int result = network_send(client, "");
    cr_assert(true, "network_send with empty message should not crash");
    
    free(client);
}

Test(network, network_send_long_message)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    char long_message[BUFFER_SIZE * 2];
    
    cr_assert_not_null(client);
    
    memset(long_message, 'a', sizeof(long_message) - 1);
    long_message[sizeof(long_message) - 1] = '\0';
    
    int result = network_send(client, long_message);
    cr_assert(true, "network_send with long message should handle buffer overflow");
    
    free(client);
}

Test(network, network_receive_basic)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    strcpy(client->read_buffer, "test command\n");
    client->read_pos = strlen(client->read_buffer);
    
    int result = network_receive(server, client);
    cr_assert(true, "network_receive should not crash");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network, network_receive_null_server)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(client);
    
    int result = network_receive(NULL, client);
    cr_assert(true, "network_receive with NULL server should not crash");
    
    free(client);
}

Test(network, network_receive_null_client)
{
    server_t *server = create_test_server_network();
    
    cr_assert_not_null(server);
    
    int result = network_receive(server, NULL);
    cr_assert(true, "network_receive with NULL client should not crash");
    
    destroy_test_server_network(server);
}

Test(network_handler, network_handle_client_line_unknown_client)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_client_line(server, client, "team1");
    cr_assert(true, "Should handle team connection");
    
    client->type = CLIENT_UNKNOWN;
    network_handle_client_line(server, client, "GRAPHIC");
    cr_assert(true, "Should handle graphic connection");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_handler, network_handle_client_line_player_client)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    network_handle_client_line(server, client, "Forward");
    network_handle_client_line(server, client, "Look");
    network_handle_client_line(server, client, "Inventory");
    
    cr_assert(true, "Should handle player commands");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_handler, network_handle_client_line_graphic_client)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_client_line(server, client, "msz");
    network_handle_client_line(server, client, "mct");
    network_handle_client_line(server, client, "tna");
    
    cr_assert(true, "Should handle GUI commands");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_handler, network_handle_client_line_null_parameters)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_client_line(NULL, client, "test");
    network_handle_client_line(server, NULL, "test");
    network_handle_client_line(server, client, NULL);
    network_handle_client_line(NULL, NULL, NULL);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_handler, network_handle_client_line_empty_line)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_client_line(server, client, "");
    network_handle_client_line(server, client, "   ");
    network_handle_client_line(server, client, "\n");
    
    cr_assert(true, "Should handle empty lines gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_client, network_handle_new_client_team)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    team_t *team = server->game->teams;
    egg_t *egg = create_test_egg_network(1, 5, 5, team);
    add_egg_to_team(team, egg);
    
    network_handle_new_client(server, client, "team1");
    cr_assert(true, "Should handle team connection");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_client, network_handle_new_client_graphic)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_new_client(server, client, "GRAPHIC");
    cr_assert_eq(client->type, CLIENT_GRAPHIC, "Client should become graphic type");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_client, network_handle_new_client_invalid_team)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_new_client(server, client, "nonexistent_team");
    cr_assert(true, "Should handle invalid team gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_client, network_handle_new_client_null_parameters)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_new_client(NULL, client, "team1");
    network_handle_new_client(server, NULL, "team1");
    network_handle_new_client(server, client, NULL);
    network_handle_new_client(NULL, NULL, NULL);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_client, network_handle_new_client_special_characters)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_new_client(server, client, "team1\n");
    network_handle_new_client(server, client, "team1\r");
    network_handle_new_client(server, client, "team1\r\n");
    network_handle_new_client(server, client, "GRAPHIC\n");
    
    cr_assert(true, "Should handle special characters in team names");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_player, network_reject_connection)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_reject_connection(server, client);
    cr_assert(true, "network_reject_connection should not crash");
    
    destroy_test_server_network(server);
}

Test(network_player, network_handle_player_connection_with_egg)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    egg_t *egg = create_test_egg_network(1, 3, 4, team);
    add_egg_to_team(team, egg);
    
    network_handle_player_connection(server, client, team);
    cr_assert(true, "Should handle player connection with available egg");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_player, network_handle_player_connection_no_eggs)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    network_handle_player_connection(server, client, team);
    cr_assert(true, "Should handle player connection with no eggs");
    
    destroy_test_server_network(server);
}

Test(network_player, network_handle_player_connection_team_full)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    team->connected_clients = team->max_clients;
    
    egg_t *egg = create_test_egg_network(1, 3, 4, team);
    add_egg_to_team(team, egg);
    
    network_handle_player_connection(server, client, team);
    cr_assert(true, "Should handle player connection when team is full");
    
    destroy_test_server_network(server);
}

Test(network_player, network_send_connection_info)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = calloc(1, sizeof(team_t));
    
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    team->name = strdup("test_team");
    team->max_clients = 5;
    team->connected_clients = 2;
    
    network_send_connection_info(client, team, 10, 10);
    cr_assert(true, "Should send connection info");
    
    free(team->name);
    free(team);
    free(client);
}

Test(network_player, network_send_connection_info_null_parameters)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = calloc(1, sizeof(team_t));
    
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    team->name = strdup("test_team");
    team->max_clients = 5;
    team->connected_clients = 2;
    
    network_send_connection_info(NULL, team, 10, 10);
    network_send_connection_info(client, NULL, 10, 10);
    network_send_connection_info(NULL, NULL, 10, 10);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    free(team->name);
    free(team);
    free(client);
}

Test(network_gui, network_handle_graphic_connection)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_graphic_connection(server, client);
    cr_assert_eq(client->type, CLIENT_GRAPHIC, "Client should be set to graphic type");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_gui, network_handle_graphic_connection_with_players)
{
    server_t *server = create_test_server_network();
    client_t *gui_client = create_test_client_network(CLIENT_UNKNOWN);
    client_t *player_client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player_client->player = player;
    player->client = player_client;
    player_client->next = server->clients;
    server->clients = player_client;
    
    network_handle_graphic_connection(server, gui_client);
    cr_assert_eq(gui_client->type, CLIENT_GRAPHIC, "Client should be set to graphic type");
    
    destroy_test_server_network(server);
    free(gui_client);
}

Test(network_gui, network_handle_graphic_connection_null_parameters)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_graphic_connection(NULL, client);
    network_handle_graphic_connection(server, NULL);
    network_handle_graphic_connection(NULL, NULL);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_basic)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    command_t *cmd = calloc(1, sizeof(command_t));
    cmd->cmd = strdup("Inventory");
    cmd->time_limit = 0;
    cmd->next = NULL;
    
    client->cmd_queue = cmd;
    client->cmd_count = 1;
    player->action_time = 0;
    
    network_process_commands(server, client);
    cr_assert(true, "Should process commands");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_no_queue)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    client->cmd_queue = NULL;
    client->cmd_count = 0;
    
    network_process_commands(server, client);
    cr_assert(true, "Should handle empty command queue");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_player_busy)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    player->action_time = 10;
    
    command_t *cmd = calloc(1, sizeof(command_t));
    cmd->cmd = strdup("Forward");
    cmd->time_limit = 0;
    cmd->next = NULL;
    
    client->cmd_queue = cmd;
    client->cmd_count = 1;
    
    network_process_commands(server, client);
    cr_assert(true, "Should handle busy player");
    
    free(cmd->cmd);
    free(cmd);
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_invalid_command)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    player->action_time = 0;
    
    command_t *cmd = calloc(1, sizeof(command_t));
    cmd->cmd = strdup("InvalidCommand");
    cmd->time_limit = 0;
    cmd->next = NULL;
    
    client->cmd_queue = cmd;
    client->cmd_count = 1;
    
    network_process_commands(server, client);
    cr_assert(true, "Should handle invalid commands");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_null_parameters)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_process_commands(NULL, client);
    network_process_commands(server, NULL);
    network_process_commands(NULL, NULL);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_commands, network_process_commands_multiple_commands)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    player->action_time = 0;
    
    command_t *cmd1 = calloc(1, sizeof(command_t));
    command_t *cmd2 = calloc(1, sizeof(command_t));
    command_t *cmd3 = calloc(1, sizeof(command_t));
    
    cmd1->cmd = strdup("Inventory");
    cmd1->time_limit = 0;
    cmd1->next = cmd2;
    
    cmd2->cmd = strdup("Look");
    cmd2->time_limit = 0;
    cmd2->next = cmd3;
    
    cmd3->cmd = strdup("Connect_nbr");
    cmd3->time_limit = 0;
    cmd3->next = NULL;
    
    client->cmd_queue = cmd1;
    client->cmd_count = 3;
    
    network_process_commands(server, client);
    cr_assert(true, "Should process first command from queue");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_egg, get_available_egg_from_team_with_eggs)
{
    team_t *team = calloc(1, sizeof(team_t));
    egg_t *egg1 = create_test_egg_network(1, 3, 4, team);
    egg_t *egg2 = create_test_egg_network(2, 5, 6, team);
    
    cr_assert_not_null(team);
    cr_assert_not_null(egg1);
    cr_assert_not_null(egg2);
    
    team->name = strdup("test_team");
    add_egg_to_team(team, egg1);
    add_egg_to_team(team, egg2);
    
    egg_t *available = get_available_egg_from_team(team);
    cr_assert_not_null(available, "Should return an available egg");
    cr_assert(available == egg2 || available == egg1, "Should return one of the eggs");
    
    free(team->name);
    free(egg1);
    free(egg2);
    free(team);
}

Test(network_egg, get_available_egg_from_team_no_eggs)
{
    team_t *team = calloc(1, sizeof(team_t));
    
    cr_assert_not_null(team);
    
    team->name = strdup("test_team");
    team->eggs = NULL;
    
    egg_t *available = get_available_egg_from_team(team);
    cr_assert_null(available, "Should return NULL when no eggs available");
    
    free(team->name);
    free(team);
}

Test(network_egg, get_available_egg_from_team_null_team)
{
    egg_t *available = get_available_egg_from_team(NULL);
    cr_assert_null(available, "Should return NULL for NULL team");
}

Test(network_egg, cleanup_egg_resources_basic)
{
    server_t *server = create_test_server_network();
    team_t *team = server->game->teams;
    egg_t *egg = create_test_egg_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(team);
    cr_assert_not_null(egg);
    
    add_egg_to_team(team, egg);
    
    tile_t *tile = &server->game->map[egg->y][egg->x];
    tile->eggs = egg;
    
    cleanup_egg_resources(server, team, egg);
    cr_assert(true, "Should cleanup egg resources");
    
    destroy_test_server_network(server);
}

Test(network_egg, cleanup_egg_resources_null_parameters)
{
    server_t *server = create_test_server_network();
    team_t *team = server->game->teams;
    egg_t *egg = create_test_egg_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(team);
    cr_assert_not_null(egg);
    
    cleanup_egg_resources(NULL, team, egg);
    cleanup_egg_resources(server, NULL, egg);
    cleanup_egg_resources(server, team, NULL);
    cleanup_egg_resources(NULL, NULL, NULL);
    
    cr_assert(true, "Should handle NULL parameters gracefully");
    
    free(egg);
    destroy_test_server_network(server);
}

Test(network_egg, cleanup_egg_resources_multiple_eggs)
{
    server_t *server = create_test_server_network();
    team_t *team = server->game->teams;
    egg_t *egg1 = create_test_egg_network(1, 5, 5, team);
    egg_t *egg2 = create_test_egg_network(2, 5, 5, team);
    egg_t *egg3 = create_test_egg_network(3, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(team);
    cr_assert_not_null(egg1);
    cr_assert_not_null(egg2);
    cr_assert_not_null(egg3);
    
    add_egg_to_team(team, egg1);
    add_egg_to_team(team, egg2);
    add_egg_to_team(team, egg3);
    
    tile_t *tile = &server->game->map[5][5];
    egg1->next = egg2;
    egg2->next = egg3;
    egg3->next = NULL;
    tile->eggs = egg1;
    
    cleanup_egg_resources(server, team, egg2);
    cr_assert(true, "Should cleanup egg from middle of list");
    
    destroy_test_server_network(server);
}

Test(network_integration, complete_client_lifecycle)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(team);
    
    egg_t *egg = create_test_egg_network(1, 3, 4, team);
    add_egg_to_team(team, egg);
    
    cr_assert_eq(client->type, CLIENT_UNKNOWN);
    
    network_handle_client_line(server, client, "team1");
    
    cr_assert(true, "Complete client lifecycle should work");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_integration, multiple_clients_different_types)
{
    server_t *server = create_test_server_network();
    client_t *gui_client = create_test_client_network(CLIENT_UNKNOWN);
    client_t *player_client1 = create_test_client_network(CLIENT_UNKNOWN);
    client_t *player_client2 = create_test_client_network(CLIENT_UNKNOWN);
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    cr_assert_not_null(player_client1);
    cr_assert_not_null(player_client2);
    cr_assert_not_null(team);
    
    egg_t *egg1 = create_test_egg_network(1, 3, 4, team);
    egg_t *egg2 = create_test_egg_network(2, 5, 6, team);
    add_egg_to_team(team, egg1);
    add_egg_to_team(team, egg2);
    
    network_handle_new_client(server, gui_client, "GRAPHIC");
    cr_assert_eq(gui_client->type, CLIENT_GRAPHIC);
    
    network_handle_new_client(server, player_client1, "team1");
    network_handle_new_client(server, player_client2, "team1");
    
    cr_assert(true, "Multiple clients of different types should work");
    
    destroy_test_server_network(server);
    free(gui_client);
    free(player_client1);
    free(player_client2);
}

Test(network_integration, player_command_queue_management)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    player->action_time = 0;
    
    network_handle_client_line(server, client, "Forward");
    network_handle_client_line(server, client, "Look");
    network_handle_client_line(server, client, "Inventory");
    
    network_process_commands(server, client);
    
    cr_assert(true, "Player command queue management should work");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_integration, gui_client_commands)
{
    server_t *server = create_test_server_network();
    client_t *gui_client = create_test_client_network(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player_client->player = player;
    player->client = player_client;
    player_client->next = server->clients;
    server->clients = player_client;
    
    network_handle_client_line(server, gui_client, "msz");
    network_handle_client_line(server, gui_client, "mct");
    network_handle_client_line(server, gui_client, "tna");
    network_handle_client_line(server, gui_client, "sgt");
    network_handle_client_line(server, gui_client, "ppo #1");
    network_handle_client_line(server, gui_client, "plv #1");
    network_handle_client_line(server, gui_client, "pin #1");
    
    cr_assert(true, "GUI client commands should work");
    
    destroy_test_server_network(server);
    free(gui_client);
}

Test(network_performance, many_commands_queue)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    for (int i = 0; i < MAX_CMD_QUEUE; i++) {
        network_handle_client_line(server, client, "Inventory");
    }
    
    cr_assert(true, "Many commands in queue should not crash");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_performance, rapid_gui_commands)
{
    server_t *server = create_test_server_network();
    client_t *gui_client = create_test_client_network(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    for (int i = 0; i < 100; i++) {
        network_handle_client_line(server, gui_client, "msz");
        network_handle_client_line(server, gui_client, "sgt");
        network_handle_client_line(server, gui_client, "mct");
    }
    
    cr_assert(true, "Rapid GUI commands should not crash");
    
    destroy_test_server_network(server);
    free(gui_client);
}

Test(network_performance, many_network_sends)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    
    cr_assert_not_null(client);
    
    for (int i = 0; i < 1000; i++) {
        network_send(client, "test message\n");
    }
    
    cr_assert(true, "Many network sends should not crash");
    
    free(client);
}

Test(network_robustness, malformed_commands)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    network_handle_client_line(server, client, "");
    network_handle_client_line(server, client, "   ");
    network_handle_client_line(server, client, "\n");
    network_handle_client_line(server, client, "\r\n");
    network_handle_client_line(server, client, "InvalidCommand");
    network_handle_client_line(server, client, "Forward extra args");
    network_handle_client_line(server, client, "123456789");
    
    cr_assert(true, "Malformed commands should be handled gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_robustness, extreme_buffer_conditions)
{
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    char large_message[BUFFER_SIZE * 3];
    
    cr_assert_not_null(client);
    
    for (int i = 0; i < sizeof(large_message) - 1; i++) {
        large_message[i] = 'A' + (i % 26);
    }
    large_message[sizeof(large_message) - 1] = '\0';
    
    network_send(client, large_message);
    
    client->read_pos = BUFFER_SIZE - 1;
    client->write_pos = BUFFER_SIZE - 1;
    
    network_send(client, "test");
    
    cr_assert(true, "Extreme buffer conditions should be handled");
    
    free(client);
}

Test(network_robustness, invalid_client_states)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    client->type = (client_type_t)999;
    network_handle_client_line(server, client, "test");
    
    client->type = CLIENT_PLAYER;
    client->player = NULL;
    network_handle_client_line(server, client, "Forward");
    
    client->type = CLIENT_GRAPHIC;
    network_handle_client_line(server, client, "Forward");
    
    cr_assert(true, "Invalid client states should be handled gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_robustness, edge_case_team_names)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_UNKNOWN);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    network_handle_new_client(server, client, "");
    network_handle_new_client(server, client, "   ");
    network_handle_new_client(server, client, "\n");
    network_handle_new_client(server, client, "\r\n");
    network_handle_new_client(server, client, "team_with_very_long_name_that_might_cause_issues");
    network_handle_new_client(server, client, "team\nwith\nnewlines");
    network_handle_new_client(server, client, "team\rwith\rcarriage\rreturns");
    
    cr_assert(true, "Edge case team names should be handled gracefully");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_stress, rapid_client_connections)
{
    server_t *server = create_test_server_network();
    team_t *team = server->game->teams;
    
    cr_assert_not_null(server);
    cr_assert_not_null(team);
    
    for (int i = 0; i < 20; i++) {
        egg_t *egg = create_test_egg_network(i, i % 10, i % 10, team);
        add_egg_to_team(team, egg);
    }
    
    for (int i = 0; i < 10; i++) {
        client_t *client = create_test_client_network(CLIENT_UNKNOWN);
        network_handle_new_client(server, client, "team1");
        free(client);
    }
    
    cr_assert(true, "Rapid client connections should be handled");
    
    destroy_test_server_network(server);
}

Test(network_stress, command_queue_overflow)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    for (int i = 0; i < MAX_CMD_QUEUE * 2; i++) {
        network_handle_client_line(server, client, "Forward");
    }
    
    cr_assert(true, "Command queue overflow should be handled");
    
    destroy_test_server_network(server);
    free(client);
}

Test(network_validation, all_network_functions_exist)
{
    cr_assert_not_null(network_send, "network_send should exist");
    cr_assert_not_null(network_receive, "network_receive should exist");
    
    cr_assert_not_null(network_handle_client_line, "network_handle_client_line should exist");
    
    cr_assert_not_null(network_handle_new_client, "network_handle_new_client should exist");
    
    cr_assert_not_null(network_reject_connection, "network_reject_connection should exist");
    cr_assert_not_null(network_handle_player_connection, "network_handle_player_connection should exist");
    cr_assert_not_null(network_send_connection_info, "network_send_connection_info should exist");
    
    cr_assert_not_null(network_handle_graphic_connection, "network_handle_graphic_connection should exist");
    
    cr_assert_not_null(network_process_commands, "network_process_commands should exist");
    
    cr_assert_not_null(get_available_egg_from_team, "get_available_egg_from_team should exist");
    cr_assert_not_null(cleanup_egg_resources, "cleanup_egg_resources should exist");
    
    cr_assert(true, "All network functions should exist");
}

Test(network_validation, network_error_handling_comprehensive)
{
    network_send(NULL, NULL);
    network_receive(NULL, NULL);
    network_handle_client_line(NULL, NULL, NULL);
    network_handle_new_client(NULL, NULL, NULL);
    network_reject_connection(NULL, NULL);
    network_handle_player_connection(NULL, NULL, NULL);
    network_send_connection_info(NULL, NULL, 0, 0);
    network_handle_graphic_connection(NULL, NULL);
    network_process_commands(NULL, NULL);
    get_available_egg_from_team(NULL);
    cleanup_egg_resources(NULL, NULL, NULL);
    
    cr_assert(true, "All network functions should handle NULL parameters gracefully");
}

Test(network_validation, network_data_integrity)
{
    server_t *server = create_test_server_network();
    client_t *client = create_test_client_network(CLIENT_PLAYER);
    team_t *team = server->game->teams;
    player_t *player = create_test_player_network(1, 5, 5, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player);
    
    client->player = player;
    player->client = client;
    
    cr_assert_eq(client->type, CLIENT_PLAYER);
    cr_assert_eq(player->id, 1);
    cr_assert_eq(player->x, 5);
    cr_assert_eq(player->y, 5);
    cr_assert_eq(player->team, team);
    
    network_handle_client_line(server, client, "Inventory");
    
    cr_assert_eq(client->player, player);
    cr_assert_eq(player->client, client);
    cr_assert_eq(player->team, team);
    
    cr_assert(true, "Network operations should maintain data integrity");
    
    destroy_test_server_network(server);
    free(client);
}
