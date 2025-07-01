/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** args_parser
*/

/**
 * @file args_parser.c
 * @brief Gère le parsing des arguments de ligne de commande pour le serveur Zappy.
 */

#include "parser.h"

/**
 * @brief Affiche le message d'utilisation du programme.
 *
 * @param prog_name Le nom de l'exécutable (typiquement argv[0]).
 */
void print_usage(const char *prog_name)
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

/**
 * @brief Valide les arguments fournis après parsing.
 *
 * @param args Structure contenant les arguments parsés.
 * @return true si les arguments sont valides, false sinon.
 */
bool validate_arguments(args_t *args)
{
    if (!args->teams || !args->teams[0]) {
        fprintf(stderr, "Error: No teams specified\n");
        return false;
    }
    if (args->width < 10 || args->width > 30 ||
        args->height < 10 || args->height > 30) {
        fprintf(stderr, "Error: Invalid world dimensions (10-30)\n");
        return false;
    }
    if (args->freq < 2 || args->freq > 10000) {
        fprintf(stderr, "Error: Invalid frequency (2-10000)\n");
        return false;
    }
    return true;
}

/**
 * @brief Affiche la configuration actuelle du serveur.
 *
 * @param args Structure contenant les arguments de configuration.
 */
void print_configuration(args_t *args)
{
    int i = 0;

    printf("Server started on port %d\n", args->port);
    printf("World size: %dx%d\n", args->width, args->height);
    printf("Teams: ");
    for (i = 0; args->teams[i]; i++)
        printf("%s ", args->teams[i]);
    printf("\n");
    printf("Max clients per team: %d\n", args->clients_nb);
    printf("Time unit: 1/%d seconds\n", args->freq);
}

/**
 * @brief Initialise les valeurs par défaut des arguments.
 *
 * @param args Structure d’arguments à initialiser.
 */
static void init_default_args(args_t *args)
{
    args->port = DEFAULT_PORT;
    args->width = DEFAULT_WIDTH;
    args->height = DEFAULT_HEIGHT;
    args->clients_nb = 3;
    args->freq = DEFAULT_FREQ;
    args->teams = NULL;
}

/**
 * @brief Parse les arguments de la ligne de commande et les stocke dans la structure fournie.
 *
 * @param argc Nombre d’arguments.
 * @param argv Tableau d’arguments.
 * @param args Structure dans laquelle stocker les résultats.
 * @return 0 si succès, sinon code d’erreur.
 */
int parse_arguments(int argc, char **argv, args_t *args)
{
    int opt = 0;
    int ret = 0;
    parse_ctx_t ctx = {0};

    init_default_args(args);
    opt = getopt(argc, argv, "p:x:y:n:c:f:h");
    while (opt != -1) {
        ctx.opt = opt;
        ctx.optarg = optarg;
        ctx.optind = &optind;
        ctx.argc = argc;
        ctx.argv = argv;
        ret = process_single_option(args, &ctx);
        if (ret != 0)
            return ret;
        opt = getopt(argc, argv, "p:x:y:n:c:f:h");
    }
    return 0;
}
