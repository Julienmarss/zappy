/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Parser header file
*/

#ifndef PARSER_H_
    #define PARSER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include <getopt.h>

    #define DEFAULT_PORT 4242
    #define DEFAULT_WIDTH 10
    #define DEFAULT_HEIGHT 10
    #define DEFAULT_FREQ 100

typedef struct {
    int port;
    int width;
    int height;
    char **teams;
    int clients_nb;
    int freq;
} args_t;

typedef struct {
    int opt;
    char *optarg;
    int *optind;
    int argc;
    char **argv;
} parse_ctx_t;

// args_parser.c
int parse_arguments(int argc, char **argv, args_t *args);
bool validate_arguments(args_t *args);
void print_usage(const char *prog_name);
void print_configuration(args_t *args);

// args_teams.c
int process_single_option(args_t *args, parse_ctx_t *ctx);

// utils.c
void free_array(char **array);

#endif /* !PARSER_H_ */
