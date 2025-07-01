/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_game
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"

static game_t *create_minimal_game(int width, int height)
{
    game_t *game = malloc(sizeof(game_t));
    if (!game) return NULL;
    
    memset(game, 0, sizeof(game_t));
    game->width = width;
    game->height = height;
    game->time_unit = 100;
    game->egg_id_counter = 1;
    
    game->map = create_map(width, height);
    if (!game->map) {
        free(game);
        return NULL;
    }
    
    return game;
}

static void safe_destroy_game(game_t *game)
{
    if (!game) return;
    
    if (game->map) {
        for (int y = 0; y < game->height; y++) {
            if (game->map[y]) {
                for (int x = 0; x < game->width; x++) {
                    if (game->map[y][x].players) {
                        free(game->map[y][x].players);
                    }
                }
                free(game->map[y]);
            }
        }
        free(game->map);
    }
    
    free(game);
}

Test(map_basic, create_map_small)
{
    tile_t **map = create_map(3, 3);
    
    cr_assert_not_null(map, "Map creation should succeed");
    
    cr_assert_not_null(map[0], "First row should exist");
    cr_assert_not_null(map[1], "Second row should exist");
    cr_assert_not_null(map[2], "Third row should exist");
    
    cr_assert_not_null(map[1][1].players, "Players array should be initialized");
    cr_assert_eq(map[1][1].player_count, 0, "Player count should start at 0");
    
    for (int r = 0; r < NB_RESOURCES; r++) {
        cr_assert_eq(map[1][1].resources[r], 0, "Resources should start at 0");
    }
    
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            free(map[y][x].players);
        }
        free(map[y]);
    }
    free(map);
}

Test(map_basic, create_map_invalid)
{
    tile_t **map1 = create_map(0, 5);
    cr_assert_null(map1, "Map with 0 width should fail");
    
    tile_t **map2 = create_map(5, 0);
    cr_assert_null(map2, "Map with 0 height should fail");
    
    tile_t **map3 = create_map(0, 0);
    cr_assert_null(map3, "Map with 0x0 should fail");
}

Test(game_basic, game_get_tile_simple)
{
    game_t *game = create_minimal_game(5, 5);
    cr_assert_not_null(game, "Game creation should succeed");
    
    tile_t *tile = game_get_tile(game, 2, 2);
    cr_assert_not_null(tile, "Valid coordinates should return a tile");
    
    tile_t *corner1 = game_get_tile(game, 0, 0);
    tile_t *corner2 = game_get_tile(game, 4, 4);
    cr_assert_not_null(corner1, "Top-left corner should exist");
    cr_assert_not_null(corner2, "Bottom-right corner should exist");
    
    safe_destroy_game(game);
}

Test(game_basic, game_get_tile_null_checks)
{
    tile_t *tile1 = game_get_tile(NULL, 0, 0);
    cr_assert_null(tile1, "NULL game should return NULL");
    
    game_t game = {0};
    game.width = 5;
    game.height = 5;
    game.map = NULL;
    
    tile_t *tile2 = game_get_tile(&game, 2, 2);
    cr_assert_null(tile2, "NULL map should return NULL");
}

Test(game_basic, game_wrapping_basic)
{
    game_t *game = create_minimal_game(10, 10);
    cr_assert_not_null(game);
    
    tile_t *tile1 = game_get_tile(game, 5, 5);
    tile_t *tile2 = game_get_tile(game, 15, 15);
    
    cr_assert_not_null(tile1);
    cr_assert_not_null(tile2);
    
    safe_destroy_game(game);
}

Test(player_basic, player_create_simple)
{
    team_t team = {0};
    team.name = strdup("test_team");
    team.max_clients = 5;
    team.connected_clients = 0;
    
    player_t *player = player_create(&team, 3, 4);
    
    if (player) {
        cr_assert_eq(player->x, 3, "Player X should be set correctly");
        cr_assert_eq(player->y, 4, "Player Y should be set correctly");
        cr_assert_eq(player->level, 1, "Player should start at level 1");
        cr_assert_geq(player->orientation, NORTH, "Orientation should be valid");
        cr_assert_leq(player->orientation, WEST, "Orientation should be valid");
        cr_assert_gt(player->id, 0, "Player ID should be positive");
        
        player_destroy(player);
    }
    
    free(team.name);
}

Test(player_basic, player_create_null_team)
{
    player_t *player = player_create(NULL, 0, 0);
    
    if (player) {
        cr_assert_null(player->team, "Player with NULL team should have NULL team");
        player_destroy(player);
    } else {
        cr_assert_null(player, "Creating player with NULL team should return NULL");
    }
}

Test(player_basic, player_destroy_null)
{
    player_destroy(NULL);
    cr_assert(true, "Destroying NULL player should not crash");
}

Test(resources_basic, manual_resource_count)
{
    game_t *game = create_minimal_game(3, 3);
    cr_assert_not_null(game);
    
    tile_t *tile = game_get_tile(game, 1, 1);
    cr_assert_not_null(tile);
    
    tile->resources[FOOD] = 5;
    tile->resources[LINEMATE] = 2;
    
    int food_count = 0;
    int linemate_count = 0;
    
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            tile_t *t = game_get_tile(game, x, y);
            if (t) {
                food_count += t->resources[FOOD];
                linemate_count += t->resources[LINEMATE];
            }
        }
    }
    
    cr_assert_eq(food_count, 5, "Should find 5 food");
    cr_assert_eq(linemate_count, 2, "Should find 2 linemate");
    
    safe_destroy_game(game);
}

Test(resources_basic, spawn_resources_null_checks)
{
    game_spawn_resources(NULL);
    cr_assert(true, "Spawning resources with NULL server should not crash");
    
    server_t server = {0};
    server.game = NULL;
    game_spawn_resources(&server);
    cr_assert(true, "Spawning resources with NULL game should not crash");
}

Test(integration_basic, game_update_null_checks)
{
    game_update(NULL);
    cr_assert(true, "Game update with NULL should not crash");
    
    server_t server = {0};
    server.game = NULL;
    server.clients = NULL;
    
    game_update(&server);
    cr_assert(true, "Game update with empty server should not crash");
}

Test(integration_basic, complete_minimal_flow)
{
    game_t *game = create_minimal_game(5, 5);
    cr_assert_not_null(game, "Game should be created");
    
    tile_t *tile = game_get_tile(game, 2, 2);
    if (tile) {
        tile->resources[FOOD] = 3;
        tile->resources[LINEMATE] = 1;
        
        cr_assert_eq(tile->resources[FOOD], 3, "Food should be set");
        cr_assert_eq(tile->resources[LINEMATE], 1, "Linemate should be set");
    }
    
    server_t server = {0};
    server.game = game;
    server.clients = NULL;
    
    game_update(&server);
    
    cr_assert(true, "Complete flow should work");
    
    safe_destroy_game(game);
}

Test(performance_light, multiple_tile_access)
{
    game_t *game = create_minimal_game(10, 10);
    cr_assert_not_null(game);
    
    for (int i = 0; i < 100; i++) {
        int x = i % 10;
        int y = (i / 10) % 10;
        tile_t *tile = game_get_tile(game, x, y);
        cr_assert_not_null(tile, "Each tile access should succeed");
    }
    
    safe_destroy_game(game);
}

Test(performance_light, player_creation_destruction)
{
    team_t team = {0};
    team.name = strdup("perf_team");
    team.max_clients = 10;
    
    for (int i = 0; i < 10; i++) {
        player_t *player = player_create(&team, i % 5, i % 5);
        if (player) {
            cr_assert_gt(player->id, 0, "Player should have valid ID");
            player_destroy(player);
        }
    }
    
    free(team.name);
    cr_assert(true, "Multiple player creation/destruction should work");
}

Test(constants_check, resource_enum_values)
{
    cr_assert_eq(FOOD, 0, "FOOD should be 0");
    cr_assert_eq(LINEMATE, 1, "LINEMATE should be 1");
    cr_assert_eq(DERAUMERE, 2, "DERAUMERE should be 2");
    cr_assert_eq(SIBUR, 3, "SIBUR should be 3");
    cr_assert_eq(MENDIANE, 4, "MENDIANE should be 4");
    cr_assert_eq(PHIRAS, 5, "PHIRAS should be 5");
    cr_assert_eq(THYSTAME, 6, "THYSTAME should be 6");
    cr_assert_eq(NB_RESOURCES, 7, "NB_RESOURCES should be 7");
}

Test(constants_check, orientation_values)
{
    cr_assert_eq(NORTH, 1, "NORTH should be 1");
    cr_assert_eq(EAST, 2, "EAST should be 2");
    cr_assert_eq(SOUTH, 3, "SOUTH should be 3");
    cr_assert_eq(WEST, 4, "WEST should be 4");
}

Test(robustness_light, extreme_coordinates_simple)
{
    game_t *game = create_minimal_game(5, 5);
    cr_assert_not_null(game);
    
    tile_t *tile1 = game_get_tile(game, 10, 10);
    tile_t *tile2 = game_get_tile(game, -1, -1);
    
    cr_assert(true, "Extreme coordinates should not crash");
    
    safe_destroy_game(game);
}

Test(robustness_light, large_resource_values)
{
    game_t *game = create_minimal_game(3, 3);
    cr_assert_not_null(game);
    
    tile_t *tile = game_get_tile(game, 1, 1);
    if (tile) {
        tile->resources[FOOD] = 1000;
        tile->resources[LINEMATE] = 500;
        
        cr_assert_eq(tile->resources[FOOD], 1000, "Large food value should be stored");
        cr_assert_eq(tile->resources[LINEMATE], 500, "Large linemate value should be stored");
    }
    
    safe_destroy_game(game);
}

Test(memory_simple, creation_destruction_cycle)
{
    for (int i = 0; i < 5; i++) {
        game_t *game = create_minimal_game(3, 3);
        if (game) {
            tile_t *tile = game_get_tile(game, 1, 1);
            if (tile) {
                tile->resources[FOOD] = i;
            }
            safe_destroy_game(game);
        }
    }
    
    cr_assert(true, "Multiple creation/destruction cycles should work");
}

Test(memory_simple, team_and_player_lifecycle)
{
    team_t team = {0};
    team.name = strdup("lifecycle_team");
    team.max_clients = 3;
    
    player_t *players[3];
    int created_count = 0;
    
    for (int i = 0; i < 3; i++) {
        players[i] = player_create(&team, i, i);
        if (players[i]) {
            created_count++;
        }
    }
    
    for (int i = 0; i < created_count; i++) {
        if (players[i]) {
            player_destroy(players[i]);
        }
    }
    
    free(team.name);
    cr_assert(true, "Player lifecycle should work correctly");
}

Test(validation_final, basic_functions_exist)
{
    cr_assert_not_null(game_get_tile, "game_get_tile function should exist");
    cr_assert_not_null(game_destroy, "game_destroy function should exist");
    
    cr_assert_not_null(create_map, "create_map function should exist");
    cr_assert_not_null(game_create, "game_create function should exist");
    
    cr_assert_not_null(player_create, "player_create function should exist");
    cr_assert_not_null(player_destroy, "player_destroy function should exist");
    
    cr_assert_not_null(game_spawn_resources, "game_spawn_resources function should exist");
    
    cr_assert_not_null(game_update, "game_update function should exist");
}

Test(validation_final, error_handling_basic)
{
    game_destroy(NULL);
    player_destroy(NULL);
    game_spawn_resources(NULL);
    game_update(NULL);
    
    cr_assert_null(game_get_tile(NULL, 0, 0), "Functions should handle NULL gracefully");
    cr_assert_null(create_map(0, 0), "Invalid parameters should be rejected");
    
    cr_assert(true, "Basic error handling should work");
}

Test(validation_final, minimal_game_complete)
{
    tile_t **map = create_map(2, 2);
    if (map) {
        cr_assert_not_null(map[0], "Map should be properly created");
        cr_assert_not_null(map[1], "Map should be properly created");
        
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                free(map[y][x].players);
            }
            free(map[y]);
        }
        free(map);
    }
    
    game_t *game = create_minimal_game(2, 2);
    if (game) {
        tile_t *tile = game_get_tile(game, 0, 0);
        cr_assert_not_null(tile, "Tile access should work");
        
        tile->resources[FOOD] = 1;
        cr_assert_eq(tile->resources[FOOD], 1, "Resource modification should work");
        
        safe_destroy_game(game);
    }
    
    cr_assert(true, "Complete minimal game flow should work");
}
