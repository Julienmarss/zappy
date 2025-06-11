/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server main entry point
*/

#include "server.h"
#include <getopt.h>

static void print_usage(const char *prog_name)
{
    printf("USAGE: %s -p port -x width -y height -n name1 name2 ... "
        "-c clientsNb -f freq\n", prog_name);
    printf("\t-p port\t\tport number\n");
    printf("\t-x width\twidth of the world\n");
    printf("\t-y height\theight of the world\n");
    printf("\t-n name1 name2 ...\tname of the team\n");
    printf("\t-c clientsNb\tnumber of authorized clients per team\n");
    printf("\t-f freq\t\treciprocal of time unit for execution of actions\n");
}

static char **allocate_teams(int count)
{
    char **teams = malloc(sizeof(char *) * (count + 1));

    if (!teams)
        return NULL;
    for (int i = 0; i <= count; i++)
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

static int parse_teams(char ***teams, int *idx, int argc, char **argv)
{
    int start = *idx;
    int count = count_team_args(idx, argc, argv);

    if (count == 0)
        return -1;
    *teams = allocate_teams(count);
    if (!*teams)
        return -1;
    for (int i = 0; i < count; i++) {
        (*teams)[i] = strdup(argv[start + i]);
        if (!(*teams)[i]) {
            free_array(*teams);
            return -1;
        }
    }
    (*idx)--;
    return count;
}

static bool validate_args(int width, int height, int freq, char **teams)
{
    if (!teams || !teams[0]) {
        fprintf(stderr, "Error: No teams specified\n");
        return false;
    }
    if (width < 10 || width > 30 || height < 10 || height > 30) {
        fprintf(stderr, "Error: Invalid world dimensions (10-30)\n");
        return false;
    }
    if (freq < 2 || freq > 10000) {
        fprintf(stderr, "Error: Invalid frequency (2-10000)\n");
        return false;
    }
    return true;
}

static void print_config(int port, int width, int height, char **teams,
    int clients_nb, int freq)
{
    printf("Server started on port %d\n", port);
    printf("World size: %dx%d\n", width, height);
    printf("Teams: ");
    for (int i = 0; teams[i]; i++)
        printf("%s ", teams[i]);
    printf("\n");
    printf("Max clients per team: %d\n", clients_nb);
    printf("Time unit: 1/%d seconds\n", freq);
}

static int parse_option(int opt, char *optarg, void *data[6])
{
    int *port = data[0];
    int *width = data[1];
    int *height = data[2];
    int *clients_nb = data[3];
    int *freq = data[4];

    if (opt == 'p')
        *port = atoi(optarg);
    else if (opt == 'x')
        *width = atoi(optarg);
    else if (opt == 'y')
        *height = atoi(optarg);
    else if (opt == 'c')
        *clients_nb = atoi(optarg);
    else if (opt == 'f')
        *freq = atoi(optarg);
    else if (opt == 'n')
        return -1;
    else
        return opt;
    return 0;
}

static int run_server(int port, int width, int height, char **teams,
    int clients_nb, int freq)
{
    t_server *server = server_create(port, width, height, teams,
        clients_nb, freq);
    int ret = 0;

    if (!server) {
        fprintf(stderr, "Error: Failed to create server\n");
        return 84;
    }
    ret = server_run(server);
    server_destroy(server);
    return ret;
}

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int clients_nb = 3;
    int freq = DEFAULT_FREQ;
    char **teams = NULL;
    void *data[6] = {&port, &width, &height, &clients_nb, &freq, &teams};

    for (int opt; (opt = getopt(argc, argv, "p:x:y:n:c:f:h")) != -1;) {
        int ret = parse_option(opt, optarg, data);
        if (ret == -1) {
            optind--;
            if (parse_teams(&teams, &optind, argc, argv) < 0)
                return 84;
        } else if (ret != 0) {
            print_usage(argv[0]);
            return (ret == 'h') ? 0 : 84;
        }
    }
    if (!validate_args(width, height, freq, teams)) {
        print_usage(argv[0]);
        free_array(teams);
        return 84;
    }
    print_config(port, width, height, teams, clients_nb, freq);
    return run_server(port, width, height, teams, clients_nb, freq);
}
