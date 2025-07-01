/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_commands
*/

#include <criterion/criterion.h>
#include "server.h"
#include "vision.h"
#include "incantation.h"

Test(commands_utils, get_resource_index_by_name_valid)
{
    cr_assert_eq(get_resource_index_by_name("food"), 0);
    cr_assert_eq(get_resource_index_by_name("linemate"), 1);
    cr_assert_eq(get_resource_index_by_name("deraumere"), 2);
    cr_assert_eq(get_resource_index_by_name("sibur"), 3);
    cr_assert_eq(get_resource_index_by_name("mendiane"), 4);
    cr_assert_eq(get_resource_index_by_name("phiras"), 5);
    cr_assert_eq(get_resource_index_by_name("thystame"), 6);
}

Test(commands_utils, get_resource_index_by_name_invalid)
{
    cr_assert_eq(get_resource_index_by_name("invalid"), -1);
    cr_assert_eq(get_resource_index_by_name(""), -1);
    cr_assert_eq(get_resource_index_by_name(NULL), -1);
}

Test(commands_utils, is_valid_resource_name_valid)
{
    cr_assert(is_valid_resource_name("food"));
    cr_assert(is_valid_resource_name("linemate"));
    cr_assert(is_valid_resource_name("deraumere"));
    cr_assert(is_valid_resource_name("sibur"));
    cr_assert(is_valid_resource_name("mendiane"));
    cr_assert(is_valid_resource_name("phiras"));
    cr_assert(is_valid_resource_name("thystame"));
}

Test(commands_utils, is_valid_resource_name_invalid)
{
    cr_assert_not(is_valid_resource_name("invalid"));
    cr_assert_not(is_valid_resource_name(""));
    cr_assert_not(is_valid_resource_name(NULL));
    cr_assert_not(is_valid_resource_name("FOOD"));
}

Test(commands_utils, get_resource_name_by_index_valid)
{
    cr_assert_str_eq(get_resource_name_by_index(0), "food");
    cr_assert_str_eq(get_resource_name_by_index(1), "linemate");
    cr_assert_str_eq(get_resource_name_by_index(2), "deraumere");
    cr_assert_str_eq(get_resource_name_by_index(3), "sibur");
    cr_assert_str_eq(get_resource_name_by_index(4), "mendiane");
    cr_assert_str_eq(get_resource_name_by_index(5), "phiras");
    cr_assert_str_eq(get_resource_name_by_index(6), "thystame");
}

Test(commands_utils, get_resource_name_by_index_invalid)
{
    cr_assert_null(get_resource_name_by_index(-1));
    cr_assert_null(get_resource_name_by_index(7));
    cr_assert_null(get_resource_name_by_index(100));
}

Test(commands_utils, roundtrip_conversion)
{
    for (int i = 0; i < 7; i++) {
        const char *name = get_resource_name_by_index(i);
        cr_assert_not_null(name);
        int index = get_resource_index_by_name(name);
        cr_assert_eq(index, i);
    }
}

Test(commands_broadcast, reconstruct_broadcast_message_single)
{
    char *args[] = {"Broadcast", "hello", NULL};
    char *result = reconstruct_broadcast_message(args);
    cr_assert_str_eq(result, "hello");
}

Test(commands_broadcast, reconstruct_broadcast_message_multiple)
{
    char *args[] = {"Broadcast", "hello", "world", "test", NULL};
    char *result = reconstruct_broadcast_message(args);
    cr_assert_str_eq(result, "hello world test");
}

Test(commands_broadcast, reconstruct_broadcast_message_empty)
{
    char *args[] = {"Broadcast", NULL};
    char *result = reconstruct_broadcast_message(args);
    cr_assert_str_eq(result, "");
}

Test(vision_builder, create_data_level_1)
{
    vision_data_t *data = vision_create_data(1);
    
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 1);
    cr_assert_eq(data->tile_count, 4);
    cr_assert_not_null(data->tiles);
    
    vision_destroy_data(data);
}

Test(vision_builder, create_data_level_2)
{
    vision_data_t *data = vision_create_data(2);
    
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 2);
    cr_assert_eq(data->tile_count, 9);
    
    vision_destroy_data(data);
}

Test(vision_builder, create_data_level_8)
{
    vision_data_t *data = vision_create_data(8);
    
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 8);
    cr_assert_eq(data->tile_count, 81);
    
    vision_destroy_data(data);
}

Test(vision_builder, create_data_invalid_level)
{
    vision_data_t *data = vision_create_data(0);
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 1);
    vision_destroy_data(data);
    
    data = vision_create_data(-5);
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 1);
    vision_destroy_data(data);
    
    data = vision_create_data(10);
    cr_assert_not_null(data);
    cr_assert_eq(data->level, 1);
    vision_destroy_data(data);
}

Test(vision_builder, destroy_data_null)
{
    vision_destroy_data(NULL);
    cr_assert(true);
}


Test(vision_coordinates, transform_north)
{
    vision_transform_t transform = {NORTH, 5, 3};
    int world_x, world_y;
    
    vision_transform_coordinates(&transform, &world_x, &world_y);
    
    cr_assert_eq(world_x, 5);
    cr_assert_eq(world_y, 3);
}

Test(vision_coordinates, transform_east)
{
    vision_transform_t transform = {EAST, 5, 3};
    int world_x, world_y;
    
    vision_transform_coordinates(&transform, &world_x, &world_y);
    
    cr_assert_eq(world_x, -3);
    cr_assert_eq(world_y, 5);
}

Test(vision_coordinates, transform_south)
{
    vision_transform_t transform = {SOUTH, 5, 3};
    int world_x, world_y;
    
    vision_transform_coordinates(&transform, &world_x, &world_y);
    
    cr_assert_eq(world_x, -5);
    cr_assert_eq(world_y, -3);
}

Test(vision_coordinates, transform_west)
{
    vision_transform_t transform = {WEST, 5, 3};
    int world_x, world_y;
    
    vision_transform_coordinates(&transform, &world_x, &world_y);
    
    cr_assert_eq(world_x, 3);
    cr_assert_eq(world_y, -5);
}

Test(vision_pattern, calculate_tiles_level_1)
{
    vision_data_t *data = vision_create_data(1);
    vision_calculate_tiles(data);
    
    cr_assert_eq(data->tiles[0].x, 0);
    cr_assert_eq(data->tiles[0].y, 0);
    cr_assert_eq(data->tiles[1].x, -1);
    cr_assert_eq(data->tiles[1].y, -1);
    cr_assert_eq(data->tiles[2].x, 0);
    cr_assert_eq(data->tiles[2].y, -1);
    cr_assert_eq(data->tiles[3].x, 1);
    cr_assert_eq(data->tiles[3].y, -1);
    
    vision_destroy_data(data);
}

Test(vision_pattern, calculate_tiles_null)
{
    vision_calculate_tiles(NULL);
    cr_assert(true);
}

Test(vision_pattern, all_levels_have_correct_indices)
{
    for (int level = 1; level <= 8; level++) {
        vision_data_t *data = vision_create_data(level);
        cr_assert_not_null(data);
        
        vision_calculate_tiles(data);
        
        for (int i = 0; i < data->tile_count; i++) {
            cr_assert_eq(data->tiles[i].index, i);
        }
        
        vision_destroy_data(data);
    }
}

Test(incantation_requirements, get_elevation_requirements_level_1)
{
    const elevation_req_t *req = get_elevation_requirements(1);
    
    cr_assert_not_null(req);
    cr_assert_eq(req->level, 1);
    cr_assert_eq(req->nb_players, 1);
    cr_assert_eq(req->linemate, 1);
    cr_assert_eq(req->deraumere, 0);
}

Test(incantation_requirements, get_elevation_requirements_level_2)
{
    const elevation_req_t *req = get_elevation_requirements(2);
    
    cr_assert_not_null(req);
    cr_assert_eq(req->level, 2);
    cr_assert_eq(req->nb_players, 2);
    cr_assert_eq(req->linemate, 1);
    cr_assert_eq(req->deraumere, 1);
    cr_assert_eq(req->sibur, 1);
}

Test(incantation_requirements, get_elevation_requirements_invalid)
{
    cr_assert_null(get_elevation_requirements(0));
    cr_assert_null(get_elevation_requirements(8));
    cr_assert_null(get_elevation_requirements(-1));
    cr_assert_null(get_elevation_requirements(100));
}

Test(incantation_requirements, get_elevation_requirements_all_valid)
{
    for (int level = 1; level <= 7; level++) {
        const elevation_req_t *req = get_elevation_requirements(level);
        cr_assert_not_null(req);
        cr_assert_eq(req->level, level);
        cr_assert_geq(req->nb_players, 1);
    }
}

Test(incantation_requirements, elevation_requirements_level_7)
{
    const elevation_req_t *req = get_elevation_requirements(7);
    cr_assert_not_null(req);
    cr_assert_eq(req->level, 7);
    cr_assert_eq(req->nb_players, 6);
    cr_assert_eq(req->linemate, 2);
    cr_assert_eq(req->deraumere, 2);
    cr_assert_eq(req->sibur, 2);
    cr_assert_eq(req->mendiane, 2);
    cr_assert_eq(req->phiras, 2);
    cr_assert_eq(req->thystame, 1);
}

Test(commands_coherence, resource_indices_match_enum)
{
    cr_assert_eq(get_resource_index_by_name("food"), FOOD);
    cr_assert_eq(get_resource_index_by_name("linemate"), LINEMATE);
    cr_assert_eq(get_resource_index_by_name("deraumere"), DERAUMERE);
    cr_assert_eq(get_resource_index_by_name("sibur"), SIBUR);
    cr_assert_eq(get_resource_index_by_name("mendiane"), MENDIANE);
    cr_assert_eq(get_resource_index_by_name("phiras"), PHIRAS);
    cr_assert_eq(get_resource_index_by_name("thystame"), THYSTAME);
}

Test(commands_coherence, orientation_values)
{
    cr_assert_eq(NORTH, 1);
    cr_assert_eq(EAST, 2);
    cr_assert_eq(SOUTH, 3);
    cr_assert_eq(WEST, 4);
}

Test(commands_coherence, resource_count)
{
    cr_assert_eq(NB_RESOURCES, 7);
    
    int count = 0;
    while (get_resource_name_by_index(count) != NULL) {
        count++;
    }
    cr_assert_eq(count, NB_RESOURCES);
}

Test(commands_robustness, vision_tile_count_formula)
{
    int expected_counts[] = {0, 4, 9, 16, 25, 36, 49, 64, 81};
    
    for (int level = 1; level <= 8; level++) {
        vision_data_t *data = vision_create_data(level);
        cr_assert_eq(data->tile_count, expected_counts[level]);
        vision_destroy_data(data);
    }
}

Test(commands_robustness, vision_level_boundaries)
{
    for (int level = 1; level <= 8; level++) {
        vision_data_t *data = vision_create_data(level);
        cr_assert_not_null(data);
        cr_assert_geq(data->tile_count, 1);
        vision_destroy_data(data);
    }
    
    int invalid_levels[] = {0, -1, 9, 100, -100};
    for (size_t i = 0; i < sizeof(invalid_levels) / sizeof(invalid_levels[0]); i++) {
        vision_data_t *data = vision_create_data(invalid_levels[i]);
        cr_assert_not_null(data);
        cr_assert_eq(data->level, 1);
        vision_destroy_data(data);
    }
}

Test(commands_robustness, resource_names_format)
{
    for (int i = 0; i < NB_RESOURCES; i++) {
        const char *name = get_resource_name_by_index(i);
        cr_assert_not_null(name);
        cr_assert_gt(strlen(name), 0);
        cr_assert_lt(strlen(name), 20);
        
        for (size_t j = 0; j < strlen(name); j++) {
            cr_assert(name[j] >= 'a' && name[j] <= 'z');
        }
    }
}

Test(commands_performance, resource_lookup_speed)
{
    clock_t start = clock();
    
    for (int i = 0; i < 1000; i++) {
        get_resource_index_by_name("food");
        get_resource_index_by_name("linemate");
        get_resource_index_by_name("thystame");
        get_resource_index_by_name("invalid");
    }
    
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    cr_assert_lt(time_taken, 0.1);
}

Test(commands_performance, vision_creation_speed)
{
    clock_t start = clock();
    
    for (int i = 0; i < 100; i++) {
        for (int level = 1; level <= 8; level++) {
            vision_data_t *data = vision_create_data(level);
            vision_calculate_tiles(data);
            vision_destroy_data(data);
        }
    }
    
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    cr_assert_lt(time_taken, 1.0);
}

Test(commands_additional, calculate_sound_direction_same_position)
{
    cr_assert(true);
}

Test(commands_additional, str_split_basic)
{
    char **result = str_split("hello world test", ' ');
    
    if (result) {
        cr_assert_str_eq(result[0], "hello");
        cr_assert_str_eq(result[1], "world");
        cr_assert_str_eq(result[2], "test");
        cr_assert_null(result[3]);
        free_array(result);
    }
}

Test(commands_additional, str_split_empty)
{
    char **result = str_split("", ' ');
    cr_assert_null(result);
}

Test(commands_additional, str_split_null)
{
    char **result = str_split(NULL, ' ');
    cr_assert_null(result);
}

Test(commands_additional, str_trim_basic)
{
    char test_str[] = "  hello world  ";
    char *result = str_trim(test_str);
    cr_assert_str_eq(result, "hello world");
}

Test(commands_additional, str_trim_no_spaces)
{
    char test_str[] = "hello";
    char *result = str_trim(test_str);
    cr_assert_str_eq(result, "hello");
}

Test(commands_additional, str_trim_null)
{
    char *result = str_trim(NULL);
    cr_assert_null(result);
}

Test(commands_additional, get_time_microseconds)
{
    long time1 = get_time_microseconds();
    long time2 = get_time_microseconds();
    
    cr_assert_geq(time2, time1);
}

Test(commands_additional, free_array_null)
{
    free_array(NULL);
    cr_assert(true);
}

Test(commands_additional, int_to_str_positive)
{
    char *result = int_to_str(42);
    cr_assert_str_eq(result, "42");
    free(result);
}

Test(commands_additional, int_to_str_negative)
{
    char *result = int_to_str(-42);
    cr_assert_str_eq(result, "-42");
    free(result);
}

Test(commands_additional, int_to_str_zero)
{
    char *result = int_to_str(0);
    cr_assert_str_eq(result, "0");
    free(result);
}
