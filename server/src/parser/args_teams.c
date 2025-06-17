/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Arguments parser implementation - Part 2 (Teams & Options)
*/

#include "parser.h"
#include <getopt.h>

static char **allocate_teams(int count)
{
    char **teams = malloc(sizeof(char *) * (count + 1));
    int i = 0;

    if (!teams)
        return NULL;
    for (i = 0; i <= count; i++)
        teams[i] = NULL;
    return teams;
}

static int count_team_args(int *idx, int argc, char **argv)
{
    int count = 0;

    while (*idx < argc && argv[*idx][0] != '-') {
        count++;
        (*idx)++;
    }
    return count;
}

static int copy_team_names(char **teams, int start, int count, char **argv)
{
    int i = 0;

    for (i = 0; i < count; i++) {
        teams[i] = strdup(argv[start + i]);
        if (!teams[i]) {
            free_array(teams);
            return -1;
        }
    }
    return 0;
}

static int parse_teams(char ***teams, int *idx, int argc, char **argv)
{
    int start = *idx;
    int count = count_team_args(idx, argc, argv);

    if (count == 0)
        return -1;
    *teams = allocate_teams(count);
    if (!*teams)
        return -1;
    if (copy_team_names(*teams, start, count, argv) != 0)
        return -1;
    (*idx)--;
    return count;
}

static void handle_numeric_options(int opt, char *optarg, args_t *args)
{
    switch (opt) {
        case 'p':
            args->port = atoi(optarg);
            break;
        case 'x':
            args->width = atoi(optarg);
            break;
        case 'y':
            args->height = atoi(optarg);
            break;
        case 'c':
            args->clients_nb = atoi(optarg);
            break;
        case 'f':
            args->freq = atoi(optarg);
            break;
    }
}

static int handle_option(int opt, char *optarg, args_t *args)
{
    if (opt == 'h')
        return 'h';
    if (opt == 'p' || opt == 'x' || opt == 'y' || opt == 'c' || opt == 'f') {
        handle_numeric_options(opt, optarg, args);
        return 0;
    }
    return 84;
}

static int handle_team_option(args_t *args, parse_ctx_t *ctx)
{
    (*(ctx->optind))--;
    if (parse_teams(&args->teams, ctx->optind, ctx->argc, ctx->argv) < 0)
        return 84;
    return 0;
}

int process_single_option(args_t *args, parse_ctx_t *ctx)
{
    int ret = 0;

    if (ctx->opt == 'n')
        return handle_team_option(args, ctx);
    ret = handle_option(ctx->opt, ctx->optarg, args);
    if (ret != 0)
        return (ret == 'h') ? 'h' : 84;
    return 0;
}
