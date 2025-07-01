/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** main
*/

/**
 * @file main.c
 * @brief Point d'entrée principal du serveur Zappy.
 */

#include "server.h"
#include "parser.h"

/**
 * @brief Initialise et lance le serveur avec les arguments fournis.
 * 
 * @param args Structure contenant les arguments du programme.
 * @return 0 en cas de succès, 84 en cas d'erreur.
 */
static int run_server_with_args(args_t *args)
{
    server_t *server = NULL;
    int ret = 0;
    server_config_t config = {
        args->port,
        args->width,
        args->height,
        args->teams,
        args->clients_nb,
        args->freq
    };

    printf("DEBUG: Creating server...\n");
    server = server_create(&config);
    if (!server) {
        fprintf(stderr, "Error: Failed to create server\n");
        return 84;
    }
    printf("DEBUG: Server created successfully, starting main loop...\n");
    ret = server_run(server);
    server_destroy(server);
    return ret;
}

/**
 * @brief Fonction principale. Gère le parsing des arguments et le lancement du serveur.
 * 
 * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Tableau de chaînes contenant les arguments.
 * @return 0 si tout se passe bien, 84 en cas d'erreur.
 */
int main(int argc, char **argv)
{
    args_t args = {0};
    int ret = 0;

    if (parse_arguments(argc, argv, &args) != 0) {
        printf("DEBUG: Parse arguments failed\n");
        print_usage(argv[0]);
        free_array(args.teams);
        return 84;
    }
    if (!validate_arguments(&args)) {
        printf("DEBUG: Argument validation failed\n");
        print_usage(argv[0]);
        free_array(args.teams);
        return 84;
    }
    print_configuration(&args);
    ret = run_server_with_args(&args);
    free_array(args.teams);
    return ret;
}
