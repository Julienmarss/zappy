/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Arguments parser header
*/

#ifndef ARGS_PARSER_H_
    #define ARGS_PARSER_H_

    #include "server.h"

typedef struct s_args {
    int port;
    int width;
    int height;
    int clients_nb;
    int freq;
    char **teams;
} args_t;

typedef struct s_parse_ctx {
    int opt;
    char *optarg;
    int *optind;
    int argc;
    char **argv;
} parse_ctx_t;

void print_usage(const char *prog_name);
int parse_arguments(int argc, char **argv, args_t *args);
bool validate_arguments(args_t *args);
void print_configuration(args_t *args);
int process_single_option(args_t *args, parse_ctx_t *ctx);

#endif /* !ARGS_PARSER_H_ */
