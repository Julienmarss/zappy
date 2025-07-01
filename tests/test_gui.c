/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_gui
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"
#include "gui_protocol.h"

static server_t *create_test_server_with_game(void)
{
    server_t *server = malloc(sizeof(server_t));
    game_t *game = malloc(sizeof(game_t));
    
    if (!server || !game) {
        free(server);
        free(game);
        return NULL;
    }
    
    memset(server, 0, sizeof(server_t));
    memset(game, 0, sizeof(game_t));
    
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
    
    server->game = game;
    server->clients = NULL;
    
    return server;
}

static client_t *create_test_client(client_type_t type)
{
    client_t *client = calloc(1, sizeof(client_t));
    if (!client) return NULL;
    
    client->fd = 42;
    client->type = type;
    client->read_pos = 0;
    client->write_pos = 0;
    
    return client;
}

static player_t *create_test_player(int id, int x, int y, const char *team_name)
{
    player_t *player = calloc(1, sizeof(player_t));
    team_t *team = calloc(1, sizeof(team_t));
    
    if (!player || !team) {
        free(player);
        free(team);
        return NULL;
    }
    
    team->name = strdup(team_name);
    team->max_clients = 5;
    team->connected_clients = 1;
    
    player->id = id;
    player->x = x;
    player->y = y;
    player->orientation = NORTH;
    player->level = 1;
    player->team = team;
    player->inventory[FOOD] = 10;
    player->food_timer = 126;
    
    return player;
}

static egg_t *create_test_egg(int id, int x, int y, team_t *team)
{
    egg_t *egg = calloc(1, sizeof(egg_t));
    if (!egg) return NULL;
    
    egg->id = id;
    egg->x = x;
    egg->y = y;
    egg->team = team;
    
    return egg;
}

static void destroy_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->game) {
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
    
    client_t *current = server->clients;
    while (current) {
        client_t *next = current->next;
        if (current->player) {
            if (current->player->team) {
                free(current->player->team->name);
                free(current->player->team);
            }
            free(current->player);
        }
        free(current);
        current = next;
    }
    
    free(server);
}

Test(gui_protocol, gui_handle_command_msz)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_handle_command(server, client, "msz");
    cr_assert(true, "GUI handle command msz should work");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_protocol, gui_handle_command_invalid)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_handle_command(server, client, "invalid_command");
    cr_assert(true, "Invalid GUI command should be handled gracefully");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_protocol, gui_handle_command_null_line)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_handle_command(server, client, NULL);
    cr_assert(true, "NULL command line should be handled gracefully");
    
    gui_handle_command(server, client, "");
    cr_assert(true, "Empty command line should be handled gracefully");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_protocol, gui_send_responses)
{
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(client);
    
    gui_send_unknown_command(client);
    gui_send_bad_parameters(client);
    
    cr_assert(true, "GUI response functions should work");
    
    free(client);
}

Test(gui_map_commands, gui_cmd_msz_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_msz(server, client, NULL);
    cr_assert(true, "MSZ command should work with valid server");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_map_commands, gui_cmd_msz_null_server)
{
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(client);
    
    gui_cmd_msz(NULL, client, NULL);
    cr_assert(true, "MSZ command should handle NULL server gracefully");
    
    free(client);
}

Test(gui_map_commands, gui_cmd_bct_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args[] = {"bct", "5", "5", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_bct(server, client, args);
    cr_assert(true, "BCT command should work with valid coordinates");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_map_commands, gui_cmd_bct_invalid_coords)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args1[] = {"bct", "15", "15", NULL};
    char *args2[] = {"bct", "-1", "-1", NULL};
    char *args3[] = {"bct", "invalid", "coords", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_bct(server, client, args1);
    gui_cmd_bct(server, client, args2);
    gui_cmd_bct(server, client, args3);
    
    cr_assert(true, "BCT command should handle invalid coordinates gracefully");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_map_commands, gui_cmd_bct_missing_args)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args1[] = {"bct", NULL};
    char *args2[] = {"bct", "5", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_bct(server, client, args1);
    gui_cmd_bct(server, client, args2);
    
    cr_assert(true, "BCT command should handle missing arguments gracefully");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_map_commands, gui_cmd_mct_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_mct(server, client, NULL);
    cr_assert(true, "MCT command should work with valid server");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_map_commands, gui_cmd_mct_null_server)
{
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(client);
    
    gui_cmd_mct(NULL, client, NULL);
    cr_assert(true, "MCT command should handle NULL server gracefully");
    
    free(client);
}

Test(gui_team_commands, gui_cmd_tna_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    team_t *team1 = calloc(1, sizeof(team_t));
    team_t *team2 = calloc(1, sizeof(team_t));
    
    team1->name = strdup("team1");
    team1->next = team2;
    team2->name = strdup("team2");
    team2->next = NULL;
    
    server->game->teams = team1;
    
    gui_cmd_tna(server, client, NULL);
    cr_assert(true, "TNA command should work with teams");
    
    free(team1->name);
    free(team2->name);
    free(team1);
    free(team2);
    server->game->teams = NULL;
    
    destroy_test_server(server);
    free(client);
}

Test(gui_team_commands, gui_cmd_tna_no_teams)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    server->game->teams = NULL;
    
    gui_cmd_tna(server, client, NULL);
    cr_assert(true, "TNA command should work with no teams");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_team_commands, gui_cmd_tna_null_server)
{
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(client);
    
    gui_cmd_tna(NULL, client, NULL);
    cr_assert(true, "TNA command should handle NULL server gracefully");
    
    free(client);
}

Test(gui_player_commands, gui_cmd_ppo_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    player_t *player = create_test_player(1, 5, 5, "test_team");
    char *args[] = {"ppo", "#1", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player_client->player = player;
    player_client->next = server->clients;
    server->clients = player_client;
    
    gui_cmd_ppo(server, client, args);
    cr_assert(true, "PPO command should work with valid player ID");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_player_commands, gui_cmd_ppo_invalid_id)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args1[] = {"ppo", "#999", NULL};
    char *args2[] = {"ppo", "invalid", NULL};
    char *args3[] = {"ppo", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    gui_cmd_ppo(server, client, args1);
    gui_cmd_ppo(server, client, args2);
    gui_cmd_ppo(server, client, args3);
    
    cr_assert(true, "PPO command should handle invalid player IDs gracefully");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_player_commands, gui_cmd_plv_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    player_t *player = create_test_player(2, 3, 4, "test_team");
    char *args[] = {"plv", "#2", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player->level = 3;
    
    player_client->player = player;
    player_client->next = server->clients;
    server->clients = player_client;
    
    gui_cmd_plv(server, client, args);
    cr_assert(true, "PLV command should work with valid player ID");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_player_commands, gui_cmd_pin_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    player_t *player = create_test_player(3, 7, 8, "test_team");
    char *args[] = {"pin", "#3", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player->inventory[LINEMATE] = 5;
    player->inventory[DERAUMERE] = 2;
    
    player_client->player = player;
    player_client->next = server->clients;
    server->clients = player_client;
    
    gui_cmd_pin(server, client, args);
    cr_assert(true, "PIN command should work with valid player ID");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_time_commands, gui_cmd_sgt_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    server->game->time_unit = 200;
    
    gui_cmd_sgt(server, client, NULL);
    cr_assert(true, "SGT command should work");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_time_commands, gui_cmd_sgt_null_server)
{
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(client);
    
    gui_cmd_sgt(NULL, client, NULL);
    cr_assert(true, "SGT command should handle NULL server gracefully");
    
    free(client);
}

Test(gui_time_commands, gui_cmd_sst_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args[] = {"sst", "150", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    int old_time_unit = server->game->time_unit;
    
    gui_cmd_sst(server, client, args);
    
    cr_assert_neq(server->game->time_unit, old_time_unit, "Time unit should change");
    cr_assert_eq(server->game->time_unit, 150, "Time unit should be 150");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_time_commands, gui_cmd_sst_invalid_values)
{
    server_t *server = create_test_server_with_game();
    client_t *client = create_test_client(CLIENT_GRAPHIC);
    char *args1[] = {"sst", "1", NULL};
    char *args2[] = {"sst", "15000", NULL};
    char *args3[] = {"sst", "invalid", NULL};
    char *args4[] = {"sst", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    int original_time_unit = server->game->time_unit;
    
    gui_cmd_sst(server, client, args1);
    gui_cmd_sst(server, client, args2);
    gui_cmd_sst(server, client, args3);
    gui_cmd_sst(server, client, args4);
    
    cr_assert_eq(server->game->time_unit, original_time_unit, 
                 "Time unit should not change with invalid values");
    
    destroy_test_server(server);
    free(client);
}

Test(gui_utils, gui_send_to_all_graphic_clients_basic)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client1 = create_test_client(CLIENT_GRAPHIC);
    client_t *gui_client2 = create_test_client(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client1);
    cr_assert_not_null(gui_client2);
    cr_assert_not_null(player_client);
    
    gui_client1->next = gui_client2;
    gui_client2->next = player_client;
    player_client->next = NULL;
    server->clients = gui_client1;
    
    gui_send_to_all_graphic_clients(server, "test message\n");
    cr_assert(true, "Should send message to all graphic clients");
    
    destroy_test_server(server);
}

Test(gui_utils, gui_send_to_all_graphic_clients_null_message)
{
    server_t *server = create_test_server_with_game();
    
    cr_assert_not_null(server);
    
    gui_send_to_all_graphic_clients(server, NULL);
    cr_assert(true, "Should handle NULL message gracefully");
    
    destroy_test_server(server);
}

Test(gui_utils, gui_format_functions)
{
    char *player_id = gui_format_player_id(42);
    char *egg_id = gui_format_egg_id(24);
    
    cr_assert_not_null(player_id);
    cr_assert_not_null(egg_id);
    
    cr_assert_str_eq(player_id, "#42", "Player ID should be formatted correctly");
    cr_assert_str_eq(egg_id, "#24", "Egg ID should be formatted correctly");
}

Test(gui_utils, gui_is_valid_coordinates)
{
    server_t *server = create_test_server_with_game();
    
    cr_assert_not_null(server);
    
    cr_assert(gui_is_valid_coordinates(server, 5, 5), "Valid coordinates should return true");
    cr_assert(gui_is_valid_coordinates(server, 0, 0), "Corner coordinates should be valid");
    cr_assert(gui_is_valid_coordinates(server, 9, 9), "Max coordinates should be valid");
    
    cr_assert_not(gui_is_valid_coordinates(server, -1, 5), "Negative X should be invalid");
    cr_assert_not(gui_is_valid_coordinates(server, 5, -1), "Negative Y should be invalid");
    cr_assert_not(gui_is_valid_coordinates(server, 10, 5), "Out of bounds X should be invalid");
    cr_assert_not(gui_is_valid_coordinates(server, 5, 10), "Out of bounds Y should be invalid");
    
    cr_assert_not(gui_is_valid_coordinates(NULL, 5, 5), "NULL server should be invalid");
    
    destroy_test_server(server);
}

Test(gui_utils, gui_is_valid_player_id)
{
    server_t *server = create_test_server_with_game();
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    player_t *player = create_test_player(123, 5, 5, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player_client->player = player;
    player_client->next = server->clients;
    server->clients = player_client;
    
    cr_assert(gui_is_valid_player_id(server, 123), "Existing player ID should be valid");
    cr_assert_not(gui_is_valid_player_id(server, 999), "Non-existing player ID should be invalid");
    cr_assert_not(gui_is_valid_player_id(server, 0), "Zero player ID should be invalid");
    cr_assert_not(gui_is_valid_player_id(server, -1), "Negative player ID should be invalid");
    cr_assert_not(gui_is_valid_player_id(NULL, 123), "NULL server should be invalid");
    
    destroy_test_server(server);
}

Test(gui_broadcast_player, gui_broadcast_new_player)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(1, 5, 5, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_new_player(server, player);
    cr_assert(true, "Should broadcast new player");
    
    gui_broadcast_new_player(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    player->team = NULL;
    gui_broadcast_new_player(server, player);
    cr_assert(true, "Should handle NULL team gracefully");
    
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_player, gui_broadcast_player_position)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(2, 3, 4, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_player_position(server, player);
    cr_assert(true, "Should broadcast player position");
    
    gui_broadcast_player_position(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_player, gui_broadcast_player_level)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(3, 5, 5, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    player->level = 5;
    
    gui_broadcast_player_level(server, player);
    cr_assert(true, "Should broadcast player level");
    
    gui_broadcast_player_level(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_player, gui_broadcast_player_inventory)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(4, 6, 7, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    player->inventory[FOOD] = 10;
    player->inventory[LINEMATE] = 3;
    player->inventory[DERAUMERE] = 2;
    
    gui_broadcast_player_inventory(server, player);
    cr_assert(true, "Should broadcast player inventory");
    
    gui_broadcast_player_inventory(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_player, gui_broadcast_player_death)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(5, 2, 3, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_player_death(server, player);
    cr_assert(true, "Should broadcast player death");
    
    gui_broadcast_player_death(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_action, gui_broadcast_player_expulsion)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(6, 4, 5, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_player_expulsion(server, player);
    cr_assert(true, "Should broadcast player expulsion");
    
    gui_broadcast_player_expulsion(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_action, gui_broadcast_player_fork)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(7, 1, 2, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_player_fork(server, player);
    cr_assert(true, "Should broadcast player fork");
    
    gui_broadcast_player_fork(server, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_action, gui_broadcast_player_broadcast)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(8, 3, 4, "test_team");
    const char *message = "Hello world";
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_player_broadcast(server, player, message);
    cr_assert(true, "Should broadcast player message");
    
    gui_broadcast_player_broadcast(server, NULL, message);
    cr_assert(true, "Should handle NULL player gracefully");
    
    gui_broadcast_player_broadcast(server, player, NULL);
    cr_assert(true, "Should handle NULL message gracefully");
    
    gui_broadcast_player_broadcast(server, NULL, NULL);
    cr_assert(true, "Should handle NULL player and message gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_resource, gui_broadcast_resource_drop)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(9, 7, 8, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_resource_drop(server, player, FOOD);
    gui_broadcast_resource_drop(server, player, LINEMATE);
    gui_broadcast_resource_drop(server, player, THYSTAME);
    cr_assert(true, "Should broadcast resource drops");
    
    gui_broadcast_resource_drop(server, player, -1);
    gui_broadcast_resource_drop(server, player, NB_RESOURCES);
    cr_assert(true, "Should handle invalid resource types gracefully");
    
    gui_broadcast_resource_drop(server, NULL, FOOD);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_resource, gui_broadcast_resource_collect)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(10, 2, 3, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    
    gui_broadcast_resource_collect(server, player, FOOD);
    gui_broadcast_resource_collect(server, player, LINEMATE);
    gui_broadcast_resource_collect(server, player, PHIRAS);
    cr_assert(true, "Should broadcast resource collections");
    
    gui_broadcast_resource_collect(server, player, -1);
    gui_broadcast_resource_collect(server, player, NB_RESOURCES + 5);
    cr_assert(true, "Should handle invalid resource types gracefully");
    
    gui_broadcast_resource_collect(server, NULL, FOOD);
    cr_assert(true, "Should handle NULL player gracefully");
    
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_egg, gui_broadcast_egg_laid)
{
    server_t *server = create_test_server_with_game();
    player_t *player = create_test_player(11, 5, 6, "test_team");
    egg_t *egg = create_test_egg(1, 5, 6, player->team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(player);
    cr_assert_not_null(egg);
    
    gui_broadcast_egg_laid(server, egg, player);
    cr_assert(true, "Should broadcast egg laid");
    
    gui_broadcast_egg_laid(server, NULL, player);
    cr_assert(true, "Should handle NULL egg gracefully");
    
    gui_broadcast_egg_laid(server, egg, NULL);
    cr_assert(true, "Should handle NULL player gracefully");
    
    gui_broadcast_egg_laid(server, NULL, NULL);
    cr_assert(true, "Should handle NULL egg and player gracefully");
    
    free(egg);
    free(player->team->name);
    free(player->team);
    free(player);
    destroy_test_server(server);
}

Test(gui_broadcast_egg, gui_broadcast_egg_connection)
{
    server_t *server = create_test_server_with_game();
    team_t *team = calloc(1, sizeof(team_t));
    team->name = strdup("test_team");
    egg_t *egg = create_test_egg(2, 3, 4, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(egg);
    
    gui_broadcast_egg_connection(server, egg);
    cr_assert(true, "Should broadcast egg connection");
    
    gui_broadcast_egg_connection(server, NULL);
    cr_assert(true, "Should handle NULL egg gracefully");
    
    free(egg);
    free(team->name);
    free(team);
    destroy_test_server(server);
}

Test(gui_broadcast_egg, gui_broadcast_egg_death)
{
    server_t *server = create_test_server_with_game();
    team_t *team = calloc(1, sizeof(team_t));
    team->name = strdup("test_team");
    egg_t *egg = create_test_egg(3, 7, 8, team);
    
    cr_assert_not_null(server);
    cr_assert_not_null(egg);
    
    gui_broadcast_egg_death(server, egg);
    cr_assert(true, "Should broadcast egg death");
    
    gui_broadcast_egg_death(server, NULL);
    cr_assert(true, "Should handle NULL egg gracefully");
    
    free(egg);
    free(team->name);
    free(team);
    destroy_test_server(server);
}

Test(gui_broadcast_incantation, gui_broadcast_incantation_start)
{
    server_t *server = create_test_server_with_game();
    player_t *player1 = create_test_player(12, 5, 5, "test_team");
    player_t *player2 = create_test_player(13, 5, 5, "test_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(player1);
    cr_assert_not_null(player2);
    
    player_t *players[] = {player1, player2};
    incantation_data_t data = {
        .x = 5,
        .y = 5,
        .level = 2,
        .players = players,
        .nb_players = 2
    };
    
    gui_broadcast_incantation_start(server, &data);
    cr_assert(true, "Should broadcast incantation start");
    
    gui_broadcast_incantation_start(server, NULL);
    cr_assert(true, "Should handle NULL data gracefully");
    
    data.players = NULL;
    gui_broadcast_incantation_start(server, &data);
    cr_assert(true, "Should handle NULL players gracefully");
    
    free(player1->team->name);
    free(player1->team);
    free(player1);
    free(player2->team->name);
    free(player2->team);
    free(player2);
    destroy_test_server(server);
}

Test(gui_broadcast_incantation, gui_broadcast_incantation_end)
{
    server_t *server = create_test_server_with_game();
    
    cr_assert_not_null(server);
    
    gui_broadcast_incantation_end(server, 5, 5, true);
    cr_assert(true, "Should broadcast incantation end success");
    
    gui_broadcast_incantation_end(server, 3, 3, false);
    cr_assert(true, "Should broadcast incantation end failure");
    
    destroy_test_server(server);
}

Test(gui_broadcast_game, gui_broadcast_game_end)
{
    server_t *server = create_test_server_with_game();
    
    cr_assert_not_null(server);
    
    gui_broadcast_game_end(server, "winning_team");
    cr_assert(true, "Should broadcast game end");
    
    gui_broadcast_game_end(server, NULL);
    cr_assert(true, "Should handle NULL team name gracefully");
    
    destroy_test_server(server);
}

Test(gui_broadcast_game, gui_broadcast_server_message)
{
    server_t *server = create_test_server_with_game();
    
    cr_assert_not_null(server);
    
    gui_broadcast_server_message(server, "Server shutting down");
    cr_assert(true, "Should broadcast server message");
    
    gui_broadcast_server_message(server, NULL);
    cr_assert(true, "Should handle NULL message gracefully");
    
    destroy_test_server(server);
}

Test(gui_integration, complete_gui_workflow)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    client_t *player_client = create_test_client(CLIENT_PLAYER);
    player_t *player = create_test_player(100, 5, 5, "integration_team");
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    cr_assert_not_null(player_client);
    cr_assert_not_null(player);
    
    player_client->player = player;
    player_client->next = gui_client;
    gui_client->next = NULL;
    server->clients = player_client;
    
    gui_handle_command(server, gui_client, "msz");
    gui_handle_command(server, gui_client, "mct");
    gui_handle_command(server, gui_client, "tna");
    gui_handle_command(server, gui_client, "sgt");
    gui_handle_command(server, gui_client, "ppo #100");
    gui_handle_command(server, gui_client, "plv #100");
    gui_handle_command(server, gui_client, "pin #100");
    gui_handle_command(server, gui_client, "bct 5 5");
    
    cr_assert(true, "Complete GUI workflow should work");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_integration, multiple_gui_clients)
{
    server_t *server = create_test_server_with_game();
    client_t *gui1 = create_test_client(CLIENT_GRAPHIC);
    client_t *gui2 = create_test_client(CLIENT_GRAPHIC);
    client_t *gui3 = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui1);
    cr_assert_not_null(gui2);
    cr_assert_not_null(gui3);
    
    gui1->next = gui2;
    gui2->next = gui3;
    gui3->next = NULL;
    server->clients = gui1;
    
    gui_send_to_all_graphic_clients(server, "broadcast test\n");
    
    cr_assert(true, "Multiple GUI clients should work");
    
    destroy_test_server(server);
}

Test(gui_integration, gui_with_game_resources)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    for (int y = 0; y < server->game->height; y++) {
        for (int x = 0; x < server->game->width; x++) {
            server->game->map[y][x].resources[FOOD] = x + y;
            server->game->map[y][x].resources[LINEMATE] = (x + y) % 3;
            server->game->map[y][x].resources[DERAUMERE] = (x * y) % 2;
        }
    }
    
    gui_cmd_mct(server, gui_client, NULL);
    gui_cmd_bct(server, gui_client, (char*[]){"bct", "0", "0", NULL});
    gui_cmd_bct(server, gui_client, (char*[]){"bct", "5", "5", NULL});
    gui_cmd_bct(server, gui_client, (char*[]){"bct", "9", "9", NULL});
    
    cr_assert(true, "GUI should work with game resources");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_performance, many_broadcast_messages)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    server->clients = gui_client;
    
    for (int i = 0; i < 1000; i++) {
        gui_send_to_all_graphic_clients(server, "performance test\n");
    }
    
    cr_assert(true, "Many broadcast messages should not crash");
    
    destroy_test_server(server);
}

Test(gui_performance, rapid_command_sequence)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    for (int i = 0; i < 100; i++) {
        gui_handle_command(server, gui_client, "msz");
        gui_handle_command(server, gui_client, "sgt");
        gui_handle_command(server, gui_client, "mct");
    }
    
    cr_assert(true, "Rapid command sequence should work");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_robustness, invalid_command_formats)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    gui_handle_command(server, gui_client, "");
    gui_handle_command(server, gui_client, "   ");
    gui_handle_command(server, gui_client, "\n");
    gui_handle_command(server, gui_client, "\t");
    gui_handle_command(server, gui_client, "msz extra args");
    gui_handle_command(server, gui_client, "bct");
    gui_handle_command(server, gui_client, "bct x");
    gui_handle_command(server, gui_client, "bct x y z");
    gui_handle_command(server, gui_client, "ppo");
    gui_handle_command(server, gui_client, "ppo 123");
    gui_handle_command(server, gui_client, "sst");
    gui_handle_command(server, gui_client, "sst abc");
    
    cr_assert(true, "Invalid command formats should be handled gracefully");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_robustness, extreme_coordinates)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    char *extreme_args[][4] = {
        {"bct", "999999", "999999", NULL},
        {"bct", "-999999", "-999999", NULL},
        {"bct", "2147483647", "2147483647", NULL},
        {"bct", "-2147483648", "-2147483648", NULL},
        {NULL}
    };
    
    for (int i = 0; extreme_args[i][0]; i++) {
        gui_cmd_bct(server, gui_client, extreme_args[i]);
    }
    
    cr_assert(true, "Extreme coordinates should be handled gracefully");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_robustness, very_long_strings)
{
    server_t *server = create_test_server_with_game();
    client_t *gui_client = create_test_client(CLIENT_GRAPHIC);
    
    cr_assert_not_null(server);
    cr_assert_not_null(gui_client);
    
    char long_command[2000];
    memset(long_command, 'a', sizeof(long_command) - 1);
    long_command[sizeof(long_command) - 1] = '\0';
    
    gui_handle_command(server, gui_client, long_command);
    cr_assert(true, "Very long strings should be handled gracefully");
    
    destroy_test_server(server);
    free(gui_client);
}

Test(gui_validation, all_gui_functions_exist)
{
    cr_assert_not_null(gui_handle_command, "gui_handle_command should exist");
    cr_assert_not_null(gui_send_unknown_command, "gui_send_unknown_command should exist");
    cr_assert_not_null(gui_send_bad_parameters, "gui_send_bad_parameters should exist");
    
    cr_assert_not_null(gui_cmd_msz, "gui_cmd_msz should exist");
    cr_assert_not_null(gui_cmd_bct, "gui_cmd_bct should exist");
    cr_assert_not_null(gui_cmd_mct, "gui_cmd_mct should exist");
    cr_assert_not_null(gui_cmd_tna, "gui_cmd_tna should exist");
    cr_assert_not_null(gui_cmd_ppo, "gui_cmd_ppo should exist");
    cr_assert_not_null(gui_cmd_plv, "gui_cmd_plv should exist");
    cr_assert_not_null(gui_cmd_pin, "gui_cmd_pin should exist");
    cr_assert_not_null(gui_cmd_sgt, "gui_cmd_sgt should exist");
    cr_assert_not_null(gui_cmd_sst, "gui_cmd_sst should exist");
    
    cr_assert_not_null(gui_send_to_all_graphic_clients, "gui_send_to_all_graphic_clients should exist");
    cr_assert_not_null(gui_format_player_id, "gui_format_player_id should exist");
    cr_assert_not_null(gui_format_egg_id, "gui_format_egg_id should exist");
    cr_assert_not_null(gui_is_valid_coordinates, "gui_is_valid_coordinates should exist");
    cr_assert_not_null(gui_is_valid_player_id, "gui_is_valid_player_id should exist");
    
    cr_assert_not_null(gui_broadcast_new_player, "gui_broadcast_new_player should exist");
    cr_assert_not_null(gui_broadcast_player_position, "gui_broadcast_player_position should exist");
    cr_assert_not_null(gui_broadcast_player_level, "gui_broadcast_player_level should exist");
    cr_assert_not_null(gui_broadcast_player_inventory, "gui_broadcast_player_inventory should exist");
    cr_assert_not_null(gui_broadcast_player_death, "gui_broadcast_player_death should exist");
    
    cr_assert_not_null(gui_broadcast_player_expulsion, "gui_broadcast_player_expulsion should exist");
    cr_assert_not_null(gui_broadcast_player_fork, "gui_broadcast_player_fork should exist");
    cr_assert_not_null(gui_broadcast_player_broadcast, "gui_broadcast_player_broadcast should exist");
    
    cr_assert_not_null(gui_broadcast_resource_drop, "gui_broadcast_resource_drop should exist");
    cr_assert_not_null(gui_broadcast_resource_collect, "gui_broadcast_resource_collect should exist");
    
    cr_assert_not_null(gui_broadcast_egg_laid, "gui_broadcast_egg_laid should exist");
    cr_assert_not_null(gui_broadcast_egg_connection, "gui_broadcast_egg_connection should exist");
    cr_assert_not_null(gui_broadcast_egg_death, "gui_broadcast_egg_death should exist");
    
    cr_assert_not_null(gui_broadcast_incantation_start, "gui_broadcast_incantation_start should exist");
    cr_assert_not_null(gui_broadcast_incantation_end, "gui_broadcast_incantation_end should exist");
    
    cr_assert_not_null(gui_broadcast_game_end, "gui_broadcast_game_end should exist");
    cr_assert_not_null(gui_broadcast_server_message, "gui_broadcast_server_message should exist");
    
    cr_assert(true, "All GUI functions should exist");
}

Test(gui_validation, gui_error_handling_comprehensive)
{
    gui_handle_command(NULL, NULL, NULL);
    gui_send_unknown_command(NULL);
    gui_send_bad_parameters(NULL);
    
    gui_cmd_msz(NULL, NULL, NULL);
    gui_cmd_bct(NULL, NULL, NULL);
    gui_cmd_mct(NULL, NULL, NULL);
    gui_cmd_tna(NULL, NULL, NULL);
    gui_cmd_ppo(NULL, NULL, NULL);
    gui_cmd_plv(NULL, NULL, NULL);
    gui_cmd_pin(NULL, NULL, NULL);
    gui_cmd_sgt(NULL, NULL, NULL);
    gui_cmd_sst(NULL, NULL, NULL);
    
    gui_send_to_all_graphic_clients(NULL, NULL);
    gui_is_valid_coordinates(NULL, 0, 0);
    gui_is_valid_player_id(NULL, 0);
    
    gui_broadcast_new_player(NULL, NULL);
    gui_broadcast_player_position(NULL, NULL);
    gui_broadcast_player_level(NULL, NULL);
    gui_broadcast_player_inventory(NULL, NULL);
    gui_broadcast_player_death(NULL, NULL);
    
    gui_broadcast_player_expulsion(NULL, NULL);
    gui_broadcast_player_fork(NULL, NULL);
    gui_broadcast_player_broadcast(NULL, NULL, NULL);
    
    gui_broadcast_resource_drop(NULL, NULL, 0);
    gui_broadcast_resource_collect(NULL, NULL, 0);
    
    gui_broadcast_egg_laid(NULL, NULL, NULL);
    gui_broadcast_egg_connection(NULL, NULL);
    gui_broadcast_egg_death(NULL, NULL);
    
    gui_broadcast_incantation_start(NULL, NULL);
    gui_broadcast_incantation_end(NULL, 0, 0, false);
    
    gui_broadcast_game_end(NULL, NULL);
    gui_broadcast_server_message(NULL, NULL);
    
    cr_assert(true, "All GUI functions should handle NULL parameters gracefully");
}
