/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** args_teams
*/

/**
 * @file args_teams.c
 * @brief Gère le parsing des arguments liés aux équipes et autres options du serveur.
 */

#include "parser.h"
#include <getopt.h>

/**
 * @brief Alloue un tableau de chaînes pour stocker les noms des équipes.
 *
 * @param count Nombre d'équipes.
 * @return Un tableau alloué dynamiquement de chaînes terminées par NULL.
 */
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

/**
 * @brief Compte le nombre d'arguments correspondant aux noms d'équipe.
 *
 * @param idx Pointeur vers l'index courant dans argv.
 * @param argc Nombre total d'arguments.
 * @param argv Tableau d'arguments.
 * @return Le nombre de noms d'équipe détectés.
 */
static int count_team_args(int *idx, int argc, char **argv)
{
    int count = 0;

    while (*idx < argc && argv[*idx][0] != '-') {
        count++;
        (*idx)++;
    }
    return count;
}

/**
 * @brief Copie les noms des équipes depuis argv vers un tableau de chaînes.
 *
 * @param teams Tableau à remplir.
 * @param start Index de départ dans argv.
 * @param count Nombre de noms à copier.
 * @param argv Tableau des arguments.
 * @return 0 si succès, -1 si échec.
 */
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

/**
 * @brief Parse les arguments d’équipes à partir de argv.
 *
 * @param teams Pointeur vers le tableau à remplir.
 * @param idx Index courant dans argv.
 * @param argc Nombre total d’arguments.
 * @param argv Tableau d’arguments.
 * @return Nombre d’équipes si succès, -1 si erreur.
 */
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

/**
 * @brief Gère les options numériques du programme.
 *
 * @param opt L’option détectée.
 * @param optarg L’argument associé à cette option.
 * @param args Structure à remplir.
 */
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

/**
 * @brief Gère les options classiques du serveur (hors -n).
 *
 * @param opt L’option détectée.
 * @param optarg L’argument associé.
 * @param args Structure d’arguments.
 * @return 0 si succès, 84 si erreur, 'h' si demande d’aide.
 */
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

/**
 * @brief Gère l’option `-n` (équipes).
 *
 * @param args Structure d’arguments.
 * @param ctx Contexte de parsing.
 * @return 0 si succès, 84 si erreur.
 */
static int handle_team_option(args_t *args, parse_ctx_t *ctx)
{
    (*(ctx->optind))--;
    if (parse_teams(&args->teams, ctx->optind, ctx->argc, ctx->argv) < 0)
        return 84;
    return 0;
}

/**
 * @brief Traite une seule option de la ligne de commande.
 *
 * @param args Structure de configuration à remplir.
 * @param ctx Contexte de parsing.
 * @return 0 si succès, 84 ou 'h' sinon.
 */
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
