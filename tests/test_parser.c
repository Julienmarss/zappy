/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parser
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "parser.h"

static void cleanup_args(args_t *args)
{
    if (args && args->teams) {
        free_array(args->teams);
        args->teams = NULL;
    }
}

static char **create_test_argv(int argc, ...)
{
    va_list args;
    char **argv = malloc(sizeof(char *) * (argc + 1));
    
    if (!argv) return NULL;
    
    va_start(args, argc);
    for (int i = 0; i < argc; i++) {
        char *arg = va_arg(args, char *);
        argv[i] = strdup(arg);
    }
    argv[argc] = NULL;
    va_end(args);
    
    return argv;
}

static void free_test_argv(char **argv, int argc)
{
    if (!argv) return;
    
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

static void init_test_args(args_t *args)
{
    memset(args, 0, sizeof(args_t));
    args->port = DEFAULT_PORT;
    args->width = DEFAULT_WIDTH;
    args->height = DEFAULT_HEIGHT;
    args->clients_nb = 3;
    args->freq = DEFAULT_FREQ;
    args->teams = NULL;
}

Test(args_parser, parse_arguments_basic_valid)
{
    args_t args = {0};
    char **argv = create_test_argv(11, "zappy_server", "-p", "4242", "-x", "10", 
                                  "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "100");
    int argc = 11;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse valid arguments successfully");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_minimal_required)
{
    args_t args = {0};
    char **argv = create_test_argv(5, "zappy_server", "-n", "team1", "team2");
    int argc = 5;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse minimal required arguments");
    
    cr_assert_eq(args.port, DEFAULT_PORT, "Should use default port");
    cr_assert_eq(args.width, DEFAULT_WIDTH, "Should use default width");
    cr_assert_eq(args.height, DEFAULT_HEIGHT, "Should use default height");
    cr_assert_eq(args.freq, DEFAULT_FREQ, "Should use default frequency");
    cr_assert_eq(args.clients_nb, 3, "Should use default clients number");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_all_options)
{
    args_t args = {0};
    char **argv = create_test_argv(13, "zappy_server", "-p", "8080", "-x", "20", 
                                  "-y", "15", "-n", "alpha", "beta", "gamma", 
                                  "-c", "7", "-f", "200");
    int argc = 13;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse all options successfully");
    
    cr_assert_eq(args.port, 8080, "Should set custom port");
    cr_assert_eq(args.width, 20, "Should set custom width");
    cr_assert_eq(args.height, 15, "Should set custom height");
    cr_assert_eq(args.clients_nb, 7, "Should set custom clients number");
    cr_assert_eq(args.freq, 200, "Should set custom frequency");
    
    cr_assert_not_null(args.teams, "Should have teams");
    cr_assert_str_eq(args.teams[0], "alpha", "First team should be alpha");
    cr_assert_str_eq(args.teams[1], "beta", "Second team should be beta");
    cr_assert_str_eq(args.teams[2], "gamma", "Third team should be gamma");
    cr_assert_null(args.teams[3], "Teams array should be null terminated");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_mixed_order)
{
    args_t args = {0};
    char **argv = create_test_argv(11, "zappy_server", "-f", "150", "-n", "team1", 
                                  "team2", "-p", "9999", "-x", "25", "-y", "30", "-c", "4");
    int argc = 11;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse mixed order arguments");
    
    cr_assert_eq(args.port, 9999, "Should set port regardless of order");
    cr_assert_eq(args.width, 25, "Should set width regardless of order");
    cr_assert_eq(args.height, 30, "Should set height regardless of order");
    cr_assert_eq(args.clients_nb, 4, "Should set clients_nb regardless of order");
    cr_assert_eq(args.freq, 150, "Should set freq regardless of order");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_help_option)
{
    args_t args = {0};
    char **argv = create_test_argv(2, "zappy_server", "-h");
    int argc = 2;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 'h', "Should return 'h' for help option");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_invalid_option)
{
    args_t args = {0};
    char **argv = create_test_argv(3, "zappy_server", "-z", "invalid");
    int argc = 3;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 84, "Should return 84 for invalid option");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_missing_value)
{
    args_t args = {0};
    char **argv = create_test_argv(2, "zappy_server", "-p");
    int argc = 2;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_neq(result, 0, "Should fail when missing option value");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_single_team)
{
    args_t args = {0};
    char **argv = create_test_argv(4, "zappy_server", "-n", "solo_team");
    int argc = 4;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse single team");
    
    cr_assert_not_null(args.teams, "Should have teams array");
    cr_assert_str_eq(args.teams[0], "solo_team", "Should have correct team name");
    cr_assert_null(args.teams[1], "Should be null terminated");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_many_teams)
{
    args_t args = {0};
    char **argv = create_test_argv(12, "zappy_server", "-n", "team1", "team2", 
                                  "team3", "team4", "team5", "team6", "team7", "team8");
    int argc = 12;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse many teams");
    
    cr_assert_not_null(args.teams, "Should have teams array");
    
    for (int i = 0; i < 8; i++) {
        cr_assert_not_null(args.teams[i], "Team %d should exist", i);
    }
    cr_assert_null(args.teams[8], "Should be null terminated");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(args_parser, parse_arguments_null_args)
{
    char **argv = create_test_argv(2, "zappy_server", "-h");
    int argc = 2;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, NULL);
    cr_assert_neq(result, 0, "Should handle NULL args gracefully");
    
    free_test_argv(argv, argc);
}

Test(args_parser, validate_arguments_valid)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = malloc(sizeof(char *) * 3);
    args.teams[0] = strdup("team1");
    args.teams[1] = strdup("team2");
    args.teams[2] = NULL;
    args.width = 15;
    args.height = 20;
    args.freq = 50;
    
    bool result = validate_arguments(&args);
    cr_assert(result, "Should validate correct arguments");
    
    cleanup_args(&args);
}

Test(args_parser, validate_arguments_no_teams)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = NULL;
    
    bool result = validate_arguments(&args);
    cr_assert_not(result, "Should reject missing teams");
    
    cleanup_args(&args);
}

Test(args_parser, validate_arguments_empty_teams)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = malloc(sizeof(char *) * 1);
    args.teams[0] = NULL;
    
    bool result = validate_arguments(&args);
    cr_assert_not(result, "Should reject empty teams array");
    
    cleanup_args(&args);
}

Test(args_parser, validate_arguments_invalid_dimensions)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = malloc(sizeof(char *) * 2);
    args.teams[0] = strdup("team1");
    args.teams[1] = NULL;
    
    args.width = 5;
    args.height = 15;
    bool result1 = validate_arguments(&args);
    cr_assert_not(result1, "Should reject width too small");
    
    args.width = 15;
    args.height = 35;
    bool result2 = validate_arguments(&args);
    cr_assert_not(result2, "Should reject height too large");
    
    args.width = 5;
    args.height = 35;
    bool result3 = validate_arguments(&args);
    cr_assert_not(result3, "Should reject both dimensions invalid");
    
    cleanup_args(&args);
}

Test(args_parser, validate_arguments_invalid_frequency)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = malloc(sizeof(char *) * 2);
    args.teams[0] = strdup("team1");
    args.teams[1] = NULL;
    
    args.freq = 1;
    bool result1 = validate_arguments(&args);
    cr_assert_not(result1, "Should reject frequency too low");
    
    args.freq = 15000;
    bool result2 = validate_arguments(&args);
    cr_assert_not(result2, "Should reject frequency too high");
    
    cleanup_args(&args);
}

Test(args_parser, validate_arguments_boundary_values)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = malloc(sizeof(char *) * 2);
    args.teams[0] = strdup("team1");
    args.teams[1] = NULL;
    
    args.width = 10;
    args.height = 10;
    args.freq = 2;
    bool result1 = validate_arguments(&args);
    cr_assert(result1, "Should accept minimum valid values");
    
    args.width = 30;
    args.height = 30;
    args.freq = 10000;
    bool result2 = validate_arguments(&args);
    cr_assert(result2, "Should accept maximum valid values");
    
    cleanup_args(&args);
}

Test(args_parser, print_usage_basic, .init = cr_redirect_stdout)
{
    print_usage("test_program");
    cr_assert(true, "print_usage should not crash");
}

Test(args_parser, print_usage_null_program, .init = cr_redirect_stdout)
{
    print_usage(NULL);
    cr_assert(true, "print_usage should handle NULL program name");
}

Test(args_parser, print_configuration_basic, .init = cr_redirect_stdout)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.port = 4242;
    args.width = 10;
    args.height = 10;
    args.clients_nb = 5;
    args.freq = 100;
    args.teams = malloc(sizeof(char *) * 3);
    args.teams[0] = strdup("team1");
    args.teams[1] = strdup("team2");
    args.teams[2] = NULL;
    
    print_configuration(&args);
    cr_assert(true, "print_configuration should not crash");
    
    cleanup_args(&args);
}

Test(args_parser, print_configuration_null_teams, .init = cr_redirect_stdout)
{
    args_t args = {0};
    init_test_args(&args);
    
    args.teams = NULL;
    
    print_configuration(&args);
    cr_assert(true, "print_configuration should handle NULL teams");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_port)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    ctx.opt = 'p';
    ctx.optarg = "8080";
    
    int result = process_single_option(&args, &ctx);
    cr_assert_eq(result, 0, "Should process port option successfully");
    cr_assert_eq(args.port, 8080, "Should set correct port value");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_dimensions)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    
    ctx.opt = 'x';
    ctx.optarg = "25";
    int result1 = process_single_option(&args, &ctx);
    cr_assert_eq(result1, 0, "Should process width option successfully");
    cr_assert_eq(args.width, 25, "Should set correct width value");
    
    ctx.opt = 'y';
    ctx.optarg = "30";
    int result2 = process_single_option(&args, &ctx);
    cr_assert_eq(result2, 0, "Should process height option successfully");
    cr_assert_eq(args.height, 30, "Should set correct height value");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_clients_freq)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    
    ctx.opt = 'c';
    ctx.optarg = "7";
    int result1 = process_single_option(&args, &ctx);
    cr_assert_eq(result1, 0, "Should process clients option successfully");
    cr_assert_eq(args.clients_nb, 7, "Should set correct clients number");
    
    ctx.opt = 'f';
    ctx.optarg = "150";
    int result2 = process_single_option(&args, &ctx);
    cr_assert_eq(result2, 0, "Should process frequency option successfully");
    cr_assert_eq(args.freq, 150, "Should set correct frequency value");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_help)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    ctx.opt = 'h';
    
    int result = process_single_option(&args, &ctx);
    cr_assert_eq(result, 'h', "Should return 'h' for help option");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_teams)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    ctx.opt = 'n';
    
    char *mock_argv[] = {"program", "-n", "team1", "team2", "team3", "-p", "4242"};
    ctx.argc = 7;
    ctx.argv = mock_argv;
    int optind_value = 2;
    ctx.optind = &optind_value;
    
    int result = process_single_option(&args, &ctx);
    cr_assert_eq(result, 0, "Should process teams option successfully");
    
    if (args.teams) {
        cr_assert_not_null(args.teams[0], "Should have first team");
        cr_assert_not_null(args.teams[1], "Should have second team");
        cr_assert_not_null(args.teams[2], "Should have third team");
    }
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_invalid)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    ctx.opt = 'z';
    
    int result = process_single_option(&args, &ctx);
    cr_assert_eq(result, 84, "Should return 84 for invalid option");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_numeric_edge_cases)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    
    ctx.opt = 'p';
    ctx.optarg = "0";
    int result1 = process_single_option(&args, &ctx);
    cr_assert_eq(result1, 0, "Should process zero port");
    cr_assert_eq(args.port, 0, "Should set port to 0");
    
    ctx.opt = 'x';
    ctx.optarg = "-5";
    int result2 = process_single_option(&args, &ctx);
    cr_assert_eq(result2, 0, "Should process negative width");
    cr_assert_eq(args.width, -5, "Should set width to -5");
    
    ctx.opt = 'f';
    ctx.optarg = "999999";
    int result3 = process_single_option(&args, &ctx);
    cr_assert_eq(result3, 0, "Should process large frequency");
    cr_assert_eq(args.freq, 999999, "Should set frequency to 999999");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_non_numeric)
{
    args_t args = {0};
    init_test_args(&args);
    
    parse_ctx_t ctx = {0};
    
    ctx.opt = 'p';
    ctx.optarg = "not_a_number";
    int result = process_single_option(&args, &ctx);
    cr_assert_eq(result, 0, "Should process non-numeric (atoi returns 0)");
    cr_assert_eq(args.port, 0, "Should set port to 0 for non-numeric");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_null_context)
{
    args_t args = {0};
    init_test_args(&args);
    
    int result = process_single_option(&args, NULL);
    cr_assert_neq(result, 0, "Should handle NULL context gracefully");
    
    cleanup_args(&args);
}

Test(args_teams, process_single_option_null_args)
{
    parse_ctx_t ctx = {0};
    ctx.opt = 'p';
    ctx.optarg = "4242";
    
    int result = process_single_option(NULL, &ctx);
    cr_assert_neq(result, 0, "Should handle NULL args gracefully");
}

Test(parser_integration, complete_valid_parsing)
{
    args_t args = {0};
    char **argv = create_test_argv(11, "zappy_server", "-p", "9000", "-x", "20", 
                                  "-y", "25", "-n", "alpha", "beta", "gamma", 
                                  "-c", "8", "-f", "250");
    int argc = 11;
    
    cr_assert_not_null(argv);
    
    int parse_result = parse_arguments(argc, argv, &args);
    cr_assert_eq(parse_result, 0, "Should parse successfully");
    
    bool validate_result = validate_arguments(&args);
    cr_assert(validate_result, "Should validate successfully");
    
    cr_assert_eq(args.port, 9000, "Port should be correct");
    cr_assert_eq(args.width, 20, "Width should be correct");
    cr_assert_eq(args.height, 25, "Height should be correct");
    cr_assert_eq(args.clients_nb, 8, "Clients number should be correct");
    cr_assert_eq(args.freq, 250, "Frequency should be correct");
    
    cr_assert_not_null(args.teams, "Should have teams");
    cr_assert_str_eq(args.teams[0], "alpha", "First team should be alpha");
    cr_assert_str_eq(args.teams[1], "beta", "Second team should be beta");
    cr_assert_str_eq(args.teams[2], "gamma", "Third team should be gamma");
    cr_assert_null(args.teams[3], "Should be null terminated");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_integration, parse_then_validate_invalid)
{
    args_t args = {0};
    char **argv = create_test_argv(7, "zappy_server", "-p", "4242", "-x", "5", 
                                  "-y", "35", "-n", "team1");
    int argc = 7;
    
    cr_assert_not_null(argv);
    
    int parse_result = parse_arguments(argc, argv, &args);
    cr_assert_eq(parse_result, 0, "Should parse successfully");
    
    bool validate_result = validate_arguments(&args);
    cr_assert_not(validate_result, "Should fail validation");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_integration, realistic_server_config)
{
    args_t args = {0};
    char **argv = create_test_argv(11, "zappy_server", "-p", "4242", "-x", "15", 
                                  "-y", "15", "-n", "team_red", "team_blue", 
                                  "team_green", "-c", "10", "-f", "100");
    int argc = 11;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse realistic config");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Should validate realistic config");
    
    cr_assert_eq(args.port, 4242);
    cr_assert_eq(args.width, 15);
    cr_assert_eq(args.height, 15);
    cr_assert_eq(args.clients_nb, 10);
    cr_assert_eq(args.freq, 100);
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_integration, minimal_but_complete)
{
    args_t args = {0};
    char **argv = create_test_argv(3, "zappy_server", "-n", "solo");
    int argc = 3;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse minimal config");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Should validate minimal config");
    
    cr_assert_eq(args.port, DEFAULT_PORT);
    cr_assert_eq(args.width, DEFAULT_WIDTH);
    cr_assert_eq(args.height, DEFAULT_HEIGHT);
    cr_assert_eq(args.freq, DEFAULT_FREQ);
    cr_assert_eq(args.clients_nb, 3);
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, empty_arguments)
{
    args_t args = {0};
    char **argv = create_test_argv(1, "zappy_server");
    int argc = 1;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should handle empty arguments");
    
    bool valid = validate_arguments(&args);
    cr_assert_not(valid, "Should fail validation without teams");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, duplicate_options)
{
    args_t args = {0};
    char **argv = create_test_argv(7, "zappy_server", "-p", "4242", "-p", "8080", "-n", "team1");
    int argc = 7;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should handle duplicate options");
    
    cr_assert_eq(args.port, 8080, "Should use last port value");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, extreme_numeric_values)
{
    args_t args = {0};
    char **argv = create_test_argv(9, "zappy_server", "-p", "2147483647", 
                                  "-x", "2147483647", "-y", "2147483647", 
                                  "-c", "2147483647", "-f", "2147483647", 
                                  "-n", "team1");
    int argc = 9;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse extreme values");
    
    bool valid = validate_arguments(&args);
    cr_assert_not(valid, "Should fail validation for extreme values");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, special_team_names)
{
    args_t args = {0};
    char **argv = create_test_argv(9, "zappy_server", "-n", "team-with-dashes", 
                                  "team_with_underscores", "team123", "UPPERCASE", 
                                  "lowercase", "MiXeD");
    int argc = 9;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse special team names");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Should validate special team names");
    
    cr_assert_not_null(args.teams);
    cr_assert_str_eq(args.teams[0], "team-with-dashes");
    cr_assert_str_eq(args.teams[1], "team_with_underscores");
    cr_assert_str_eq(args.teams[2], "team123");
    cr_assert_str_eq(args.teams[3], "UPPERCASE");
    cr_assert_str_eq(args.teams[4], "lowercase");
    cr_assert_str_eq(args.teams[5], "MiXeD");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, long_team_names)
{
    args_t args = {0};
    char long_name[1000];
    memset(long_name, 'a', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    
    char **argv = create_test_argv(4, "zappy_server", "-n", long_name, "normal_team");
    int argc = 4;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse long team names");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Should validate long team names");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, mixed_valid_invalid_options)
{
    args_t args = {0};
    char **argv = create_test_argv(7, "zappy_server", "-p", "4242", "-z", "invalid", "-n", "team1");
    int argc = 7;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 84, "Should fail on invalid option");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, option_without_dash)
{
    args_t args = {0};
    char **argv = create_test_argv(4, "zappy_server", "p", "4242", "-n", "team1");
    int argc = 4;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should treat non-dash arguments as teams");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_robustness, negative_numeric_values)
{
    args_t args = {0};
    char **argv = create_test_argv(9, "zappy_server", "-p", "-1", "-x", "-10", 
                                  "-y", "-20", "-c", "-5", "-f", "-100", "-n", "team1");
    int argc = 9;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse negative values");
    
    bool valid = validate_arguments(&args);
    cr_assert_not(valid, "Should fail validation for negative values");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_performance, many_teams)
{
    args_t args = {0};
    
    int team_count = 100;
    int argc = 3 + team_count;
    char **argv = malloc(sizeof(char *) * (argc + 1));
    
    argv[0] = strdup("zappy_server");
    argv[1] = strdup("-n");
    
    for (int i = 0; i < team_count; i++) {
        char team_name[32];
        snprintf(team_name, sizeof(team_name), "team_%d", i);
        argv[2 + i] = strdup(team_name);
    }
    argv[argc] = NULL;
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse many teams");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Should validate many teams");
    
    for (int i = 0; i < team_count; i++) {
        cr_assert_not_null(args.teams[i], "Team %d should exist", i);
    }
    cr_assert_null(args.teams[team_count], "Should be null terminated");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_performance, rapid_parsing)
{
    for (int iteration = 0; iteration < 1000; iteration++) {
        args_t args = {0};
        char **argv = create_test_argv(7, "zappy_server", "-p", "4242", 
                                      "-x", "15", "-y", "15", "-n", "team1", "-c", "5");
        int argc = 7;
        
        int result = parse_arguments(argc, argv, &args);
        cr_assert_eq(result, 0, "Iteration %d should parse successfully", iteration);
        
        cleanup_args(&args);
        free_test_argv(argv, argc);
    }
    
    cr_assert(true, "Rapid parsing should complete successfully");
}

Test(parser_memory, memory_allocation_patterns)
{
    args_t args = {0};
    char **argv = create_test_argv(7, "zappy_server", "-n", "team1", "team2", "team3");
    int argc = 7;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse successfully");
    
    cr_assert_not_null(args.teams, "Teams array should be allocated");
    cr_assert_not_null(args.teams[0], "First team should be allocated");
    cr_assert_not_null(args.teams[1], "Second team should be allocated");
    cr_assert_not_null(args.teams[2], "Third team should be allocated");
    cr_assert_null(args.teams[3], "Should be null terminated");
    
    cr_assert_str_eq(args.teams[0], "team1");
    cr_assert_str_eq(args.teams[1], "team2");
    cr_assert_str_eq(args.teams[2], "team3");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_memory, repeated_allocation_cleanup)
{
    for (int i = 0; i < 100; i++) {
        args_t args = {0};
        char **argv = create_test_argv(5, "zappy_server", "-n", "team1", "team2");
        int argc = 5;
        
        int result = parse_arguments(argc, argv, &args);
        cr_assert_eq(result, 0, "Cycle %d should parse successfully", i);
        
        cleanup_args(&args);
        free_test_argv(argv, argc);
    }
    
    cr_assert(true, "Repeated allocation/cleanup should work");
}

Test(parser_memory, empty_teams_cleanup)
{
    args_t args = {0};
    
    args.teams = malloc(sizeof(char *) * 1);
    args.teams[0] = NULL;
    
    cleanup_args(&args);
    cr_assert(true, "Should cleanup empty teams gracefully");
}

Test(parser_memory, partial_allocation_failure_simulation)
{
    args_t args = {0};
    
    args.teams = malloc(sizeof(char *) * 4);
    args.teams[0] = strdup("team1");
    args.teams[1] = strdup("team2");
    args.teams[2] = NULL;
    args.teams[3] = NULL;
    
    cleanup_args(&args);
    cr_assert(true, "Should cleanup partial allocations gracefully");
}

Test(parser_edge_cases, boundary_port_values)
{
    args_t args = {0};
    
    char **argv1 = create_test_argv(5, "zappy_server", "-p", "0", "-n", "team1");
    int result1 = parse_arguments(5, argv1, &args);
    cr_assert_eq(result1, 0, "Should parse port 0");
    cr_assert_eq(args.port, 0, "Should set port to 0");
    cleanup_args(&args);
    free_test_argv(argv1, 5);
    
    char **argv2 = create_test_argv(5, "zappy_server", "-p", "65535", "-n", "team1");
    int result2 = parse_arguments(5, argv2, &args);
    cr_assert_eq(result2, 0, "Should parse port 65535");
    cr_assert_eq(args.port, 65535, "Should set port to 65535");
    cleanup_args(&args);
    free_test_argv(argv2, 5);
}

Test(parser_edge_cases, boundary_dimension_values)
{
    args_t args = {0};
    
    char **argv1 = create_test_argv(7, "zappy_server", "-x", "10", "-y", "10", "-n", "team1");
    int result1 = parse_arguments(7, argv1, &args);
    cr_assert_eq(result1, 0, "Should parse minimum dimensions");
    bool valid1 = validate_arguments(&args);
    cr_assert(valid1, "Should validate minimum dimensions");
    cleanup_args(&args);
    free_test_argv(argv1, 7);
    
    char **argv2 = create_test_argv(7, "zappy_server", "-x", "30", "-y", "30", "-n", "team1");
    int result2 = parse_arguments(7, argv2, &args);
    cr_assert_eq(result2, 0, "Should parse maximum dimensions");
    bool valid2 = validate_arguments(&args);
    cr_assert(valid2, "Should validate maximum dimensions");
    cleanup_args(&args);
    free_test_argv(argv2, 7);
    
    char **argv3 = create_test_argv(7, "zappy_server", "-x", "9", "-y", "9", "-n", "team1");
    int result3 = parse_arguments(7, argv3, &args);
    cr_assert_eq(result3, 0, "Should parse below minimum dimensions");
    bool valid3 = validate_arguments(&args);
    cr_assert_not(valid3, "Should not validate below minimum dimensions");
    cleanup_args(&args);
    free_test_argv(argv3, 7);
    
    char **argv4 = create_test_argv(7, "zappy_server", "-x", "31", "-y", "31", "-n", "team1");
    int result4 = parse_arguments(7, argv4, &args);
    cr_assert_eq(result4, 0, "Should parse above maximum dimensions");
    bool valid4 = validate_arguments(&args);
    cr_assert_not(valid4, "Should not validate above maximum dimensions");
    cleanup_args(&args);
    free_test_argv(argv4, 7);
}

Test(parser_edge_cases, boundary_frequency_values)
{
    args_t args = {0};
    
    char **argv1 = create_test_argv(5, "zappy_server", "-f", "2", "-n", "team1");
    int result1 = parse_arguments(5, argv1, &args);
    cr_assert_eq(result1, 0, "Should parse minimum frequency");
    bool valid1 = validate_arguments(&args);
    cr_assert(valid1, "Should validate minimum frequency");
    cleanup_args(&args);
    free_test_argv(argv1, 5);
    
    char **argv2 = create_test_argv(5, "zappy_server", "-f", "10000", "-n", "team1");
    int result2 = parse_arguments(5, argv2, &args);
    cr_assert_eq(result2, 0, "Should parse maximum frequency");
    bool valid2 = validate_arguments(&args);
    cr_assert(valid2, "Should validate maximum frequency");
    cleanup_args(&args);
    free_test_argv(argv2, 5);
    
    char **argv3 = create_test_argv(5, "zappy_server", "-f", "1", "-n", "team1");
    int result3 = parse_arguments(5, argv3, &args);
    cr_assert_eq(result3, 0, "Should parse below minimum frequency");
    bool valid3 = validate_arguments(&args);
    cr_assert_not(valid3, "Should not validate below minimum frequency");
    cleanup_args(&args);
    free_test_argv(argv3, 5);
    
    char **argv4 = create_test_argv(5, "zappy_server", "-f", "10001", "-n", "team1");
    int result4 = parse_arguments(5, argv4, &args);
    cr_assert_eq(result4, 0, "Should parse above maximum frequency");
    bool valid4 = validate_arguments(&args);
    cr_assert_not(valid4, "Should not validate above maximum frequency");
    cleanup_args(&args);
    free_test_argv(argv4, 5);
}

Test(parser_edge_cases, zero_clients)
{
    args_t args = {0};
    char **argv = create_test_argv(5, "zappy_server", "-c", "0", "-n", "team1");
    int argc = 5;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse zero clients");
    cr_assert_eq(args.clients_nb, 0, "Should set clients to 0");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Zero clients might be valid for testing");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_edge_cases, single_character_team_names)
{
    args_t args = {0};
    char **argv = create_test_argv(6, "zappy_server", "-n", "a", "b", "c");
    int argc = 6;
    
    cr_assert_not_null(argv);
    
    int result = parse_arguments(argc, argv, &args);
    cr_assert_eq(result, 0, "Should parse single character team names");
    
    bool valid = validate_arguments(&args);
    cr_assert(valid, "Single character team names should be valid");
    
    cr_assert_str_eq(args.teams[0], "a");
    cr_assert_str_eq(args.teams[1], "b");
    cr_assert_str_eq(args.teams[2], "c");
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
}

Test(parser_validation, all_parser_functions_exist)
{
    cr_assert_not_null(parse_arguments, "parse_arguments should exist");
    cr_assert_not_null(validate_arguments, "validate_arguments should exist");
    cr_assert_not_null(print_usage, "print_usage should exist");
    cr_assert_not_null(print_configuration, "print_configuration should exist");
    cr_assert_not_null(process_single_option, "process_single_option should exist");
    cr_assert_not_null(free_array, "free_array should exist");
    
    cr_assert(true, "All parser functions should exist");
}

Test(parser_validation, parser_error_handling_comprehensive)
{
    parse_arguments(0, NULL, NULL);
    validate_arguments(NULL);
    print_usage(NULL);
    print_configuration(NULL);
    process_single_option(NULL, NULL);
    free_array(NULL);
    
    cr_assert(true, "All parser functions should handle NULL parameters gracefully");
}

Test(parser_validation, parser_constants_verification)
{
    cr_assert_eq(DEFAULT_PORT, 4242, "DEFAULT_PORT should be 4242");
    cr_assert_eq(DEFAULT_WIDTH, 10, "DEFAULT_WIDTH should be 10");
    cr_assert_eq(DEFAULT_HEIGHT, 10, "DEFAULT_HEIGHT should be 10");
    cr_assert_eq(DEFAULT_FREQ, 100, "DEFAULT_FREQ should be 100");
    
    cr_assert(true, "Parser constants should be correct");
}

Test(parser_validation, complete_parser_workflow)
{
    args_t args = {0};
    char **argv = create_test_argv(11, "zappy_server", "-p", "8888", "-x", "20", 
                                  "-y", "25", "-n", "red", "blue", "green", 
                                  "-c", "6", "-f", "120");
    int argc = 11;
    
    cr_assert_not_null(argv);
    
    int parse_result = parse_arguments(argc, argv, &args);
    cr_assert_eq(parse_result, 0, "Parse should succeed");
    
    bool validate_result = validate_arguments(&args);
    cr_assert(validate_result, "Validation should succeed");
    
    print_configuration(&args);
    
    cr_assert_eq(args.port, 8888);
    cr_assert_eq(args.width, 20);
    cr_assert_eq(args.height, 25);
    cr_assert_eq(args.clients_nb, 6);
    cr_assert_eq(args.freq, 120);
    
    cr_assert_not_null(args.teams);
    cr_assert_str_eq(args.teams[0], "red");
    cr_assert_str_eq(args.teams[1], "blue");
    cr_assert_str_eq(args.teams[2], "green");
    cr_assert_null(args.teams[3]);
    
    cleanup_args(&args);
    free_test_argv(argv, argc);
    
    cr_assert(true, "Complete parser workflow should work");
}

Test(parser_validation, real_world_usage_scenarios)
{
    {
        args_t args = {0};
        char **argv = create_test_argv(7, "zappy_server", "-p", "4242", 
                                      "-n", "dev_team", "-c", "1", "-f", "1000");
        int result = parse_arguments(7, argv, &args);
        cr_assert_eq(result, 0);
        cr_assert(validate_arguments(&args));
        cleanup_args(&args);
        free_test_argv(argv, 7);
    }
    
    {
        args_t args = {0};
        char **argv = create_test_argv(11, "zappy_server", "-p", "8080", 
                                      "-x", "30", "-y", "30", "-n", "team_a", 
                                      "team_b", "team_c", "team_d", "-c", "20", "-f", "10");
        int result = parse_arguments(11, argv, &args);
        cr_assert_eq(result, 0);
        cr_assert(validate_arguments(&args));
        cleanup_args(&args);
        free_test_argv(argv, 11);
    }
    
    {
        args_t args = {0};
        char **argv = create_test_argv(9, "zappy_server", "-x", "10", "-y", "10", 
                                      "-n", "test1", "test2", "-c", "2", "-f", "1000");
        int result = parse_arguments(9, argv, &args);
        cr_assert_eq(result, 0);
        cr_assert(validate_arguments(&args));
        cleanup_args(&args);
        free_test_argv(argv, 9);
    }
    
    cr_assert(true, "Real world usage scenarios should work");
}
