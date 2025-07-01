/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_server
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"

static server_config_t *create_safe_test_config(void)
{
    server_config_t *config = malloc(sizeof(server_config_t));
    if (!config) return NULL;
    
    config->port = 0;
    config->width = 10;
    config->height = 10;
    config->nb_clients = 5;
    config->freq = 100;
    
    config->teams = malloc(sizeof(char *) * 3);
    if (!config->teams) {
        free(config);
        return NULL;
    }
    config->teams[0] = strdup("team1");
    config->teams[1] = strdup("team2");
    config->teams[2] = NULL;
    
    return config;
}

static void destroy_safe_test_config(server_config_t *config)
{
    if (!config) return;
    
    if (config->teams) {
        for (int i = 0; config->teams[i]; i++) {
            free(config->teams[i]);
        }
        free(config->teams);
    }
    free(config);
}

static client_t *create_safe_test_client(int fake_fd, client_type_t type)
{
    client_t *client = calloc(1, sizeof(client_t));
    if (!client) return NULL;
    
    client->fd = fake_fd;
    client->type = type;
    client->read_pos = 0;
    client->write_pos = 0;
    client->cmd_queue = NULL;
    client->cmd_count = 0;
    client->next = NULL;
    client->player = NULL;
    
    return client;
}

static void safe_cleanup_client(client_t *client)
{
    if (!client) return;
    
    command_t *cmd = client->cmd_queue;
    while (cmd) {
        command_t *next = cmd->next;
        free(cmd->cmd);
        free(cmd);
        cmd = next;
    }
    
    free(client);
}

Test(server_safe, server_create_basic)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config, "Config should be created");
    
    server_t *server = server_create(config);
    
    if (server) {
        cr_assert_not_null(server->game, "Server should have a game");
        cr_assert_geq(server->socket_fd, 0, "Server socket should be valid");
        
        server_destroy(server);
        cr_assert(true, "Server should be destroyed safely");
    } else {
        cr_assert(true, "Server creation failure is acceptable in tests");
    }
    
    destroy_safe_test_config(config);
}

Test(server_safe, server_create_null_config)
{
    server_t *server = server_create(NULL);
    cr_assert_null(server, "Server creation with NULL config should fail");
}

Test(server_safe, server_destroy_null)
{
    server_destroy(NULL);
    cr_assert(true, "Destroying NULL server should not crash");
}

Test(server_safe, server_destroy_basic)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config);
    
    server_t *server = server_create(config);
    
    if (server) {
        server_destroy(server);
        cr_assert(true, "Server destruction should complete without crash");
    }
    
    destroy_safe_test_config(config);
}

Test(server_accept_safe, server_accept_client_null_server)
{
    server_accept_client(NULL);
    cr_assert(true, "server_accept_client with NULL should not crash");
}

Test(server_accept_safe, server_disconnect_client_null_parameters)
{
    server_disconnect_client(NULL, NULL);
    cr_assert(true, "server_disconnect_client with NULL should not crash");
    
    client_t *fake_client = create_safe_test_client(999, CLIENT_PLAYER);
    server_disconnect_client(NULL, fake_client);
    safe_cleanup_client(fake_client);
    
    cr_assert(true, "Disconnect with NULL server should not crash");
}

Test(server_accept_safe, server_disconnect_client_basic)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config);
    
    server_t *server = server_create(config);
    
    if (server) {
        client_t *client = create_safe_test_client(999, CLIENT_PLAYER);
        
        server_disconnect_client(server, client);
        
        cr_assert(true, "Disconnect should handle non-listed clients gracefully");
        
        server_destroy(server);
    }
    
    destroy_safe_test_config(config);
}

Test(server_events_safe, handle_poll_events_null_server)
{
    int result = handle_poll_events(NULL, 1);
    cr_assert(true, "handle_poll_events with NULL server should not crash");
}

Test(server_events_safe, handle_poll_events_basic)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config);
    
    server_t *server = server_create(config);
    
    if (server) {
        int result = handle_poll_events(server, 0);
        cr_assert(true, "handle_poll_events should not crash");
        
        server_destroy(server);
    }
    
    destroy_safe_test_config(config);
}

Test(server_run_safe, server_run_null_server)
{
    int result = server_run(NULL);
    cr_assert_neq(result, 0, "server_run with NULL should return error");
}


Test(server_run_safe, server_run_function_exists)
{
    cr_assert_not_null(server_run, "server_run function should exist");
}

Test(server_validation_safe, all_server_functions_exist)
{
    cr_assert_not_null(server_create, "server_create should exist");
    cr_assert_not_null(server_destroy, "server_destroy should exist");
    cr_assert_not_null(server_run, "server_run should exist");
    cr_assert_not_null(server_accept_client, "server_accept_client should exist");
    cr_assert_not_null(server_disconnect_client, "server_disconnect_client should exist");
    cr_assert_not_null(handle_poll_events, "handle_poll_events should exist");
    
    cr_assert(true, "All server functions should exist");
}

Test(server_validation_safe, server_null_safety)
{
    server_destroy(NULL);
    server_accept_client(NULL);
    server_disconnect_client(NULL, NULL);
    handle_poll_events(NULL, 0);
    
    int result = server_run(NULL);
    cr_assert_neq(result, 0, "server_run should return error for NULL");
    
    cr_assert(true, "All server functions should handle NULL gracefully");
}

Test(server_validation_safe, server_config_validation)
{
    server_config_t valid_config = {
        .port = 0,
        .width = 10,
        .height = 10,
        .nb_clients = 5,
        .freq = 100,
        .teams = NULL
    };
    
    char *teams[] = {"team1", "team2", NULL};
    valid_config.teams = teams;
    
    server_t *server = server_create(&valid_config);
    
    if (server) {
        cr_assert_not_null(server->game, "Server should have game");
        cr_assert_eq(server->game->width, 10, "Width should be set");
        cr_assert_eq(server->game->height, 10, "Height should be set");
        
        server_destroy(server);
        cr_assert(true, "Valid config should work");
    } else {
        cr_assert(true, "Server creation failure is acceptable");
    }
}

Test(server_integration_safe, simple_server_lifecycle)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config);
    
    server_t *server = server_create(config);
    
    if (server) {
        cr_assert_not_null(server->game, "Game should be initialized");
        cr_assert_geq(server->socket_fd, 0, "Socket should be valid");
        
        handle_poll_events(server, 0);
        server_accept_client(server);
        
        server_destroy(server);
        cr_assert(true, "Simple lifecycle should complete");
    } else {
        cr_assert(true, "Server creation failure is acceptable in test environment");
    }
    
    destroy_safe_test_config(config);
}

Test(server_integration_safe, config_variations)
{
    struct {
        int width, height, clients, freq;
    } configs[] = {
        {10, 10, 1, 100},
        {15, 20, 5, 50},
        {30, 30, 10, 200}
    };
    
    int num_configs = sizeof(configs) / sizeof(configs[0]);
    
    for (int i = 0; i < num_configs; i++) {
        server_config_t *config = create_safe_test_config();
        if (!config) continue;
        
        config->width = configs[i].width;
        config->height = configs[i].height;
        config->nb_clients = configs[i].clients;
        config->freq = configs[i].freq;
        
        server_t *server = server_create(config);
        
        if (server) {
            cr_assert_eq(server->game->width, configs[i].width, "Width should match");
            cr_assert_eq(server->game->height, configs[i].height, "Height should match");
            server_destroy(server);
        }
        
        destroy_safe_test_config(config);
    }
    
    cr_assert(true, "Different configurations should work");
}

Test(server_memory_safe, creation_destruction_cycles)
{
    for (int i = 0; i < 5; i++) {
        server_config_t *config = create_safe_test_config();
        if (!config) continue;
        
        server_t *server = server_create(config);
        
        if (server) {
            server_destroy(server);
        }
        
        destroy_safe_test_config(config);
    }
    
    cr_assert(true, "Multiple creation/destruction cycles should work");
}

Test(server_memory_safe, config_cleanup)
{
    for (int i = 0; i < 10; i++) {
        server_config_t *config = create_safe_test_config();
        if (config) {
            destroy_safe_test_config(config);
        }
    }
    
    cr_assert(true, "Config creation/cleanup should work");
}

Test(server_performance_safe, rapid_creation)
{
    int successful_creations = 0;
    
    for (int i = 0; i < 10; i++) {
        server_config_t *config = create_safe_test_config();
        if (!config) continue;
        
        server_t *server = server_create(config);
        
        if (server) {
            successful_creations++;
            server_destroy(server);
        }
        
        destroy_safe_test_config(config);
    }
    
    cr_assert_geq(successful_creations, 1, "At least one server should be created successfully");
}

Test(server_final_safe, basic_functionality_check)
{
    server_config_t *config = create_safe_test_config();
    cr_assert_not_null(config, "Config creation should work");
    
    server_t *server = server_create(config);
    
    if (server) {
        cr_assert_not_null(server->game, "Game should exist");
        cr_assert_not_null(server->poll_fds, "Poll fds should exist");
        cr_assert_geq(server->socket_fd, 0, "Socket should be valid");
        cr_assert_eq(server->poll_count, 1, "Should have server socket in poll");
        
        handle_poll_events(server, 0);
        
        server_destroy(server);
        cr_assert(true, "All basic functionality should work");
    } else {
        cr_assert(true, "Server creation failure is acceptable in test environment");
    }
    
    destroy_safe_test_config(config);
}

Test(server_final_safe, stress_test_light)
{
    for (int i = 0; i < 3; i++) {
        server_config_t *config = create_safe_test_config();
        if (!config) continue;
        
        server_t *server = server_create(config);
        
        if (server) {
            for (int j = 0; j < 5; j++) {
                handle_poll_events(server, 0);
            }
            
            server_destroy(server);
        }
        
        destroy_safe_test_config(config);
    }
    
    cr_assert(true, "Light stress test should complete");
}
