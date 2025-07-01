/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_protocol
*/

/**
 * @file gui_protocol.c
 * @brief Gère le protocole de communication des clients graphiques (GUI).
 *
 * Ce fichier contient la logique de dispatch des commandes reçues
 * depuis l’interface graphique. Il mappe chaque commande à un handler
 * et appelle la fonction correspondante.
 */

#include "gui_protocol.h"

/// Tableau des commandes GUI et leurs handlers associés.
static const gui_handler_t GUI_HANDLERS[] = {
    {"msz", gui_cmd_msz, false},  ///< Taille de la carte
    {"bct", gui_cmd_bct, true},   ///< Contenu d’une case
    {"mct", gui_cmd_mct, false},  ///< Contenu de toute la carte
    {"tna", gui_cmd_tna, false},  ///< Liste des équipes
    {"ppo", gui_cmd_ppo, true},   ///< Position d’un joueur
    {"plv", gui_cmd_plv, true},   ///< Niveau d’un joueur
    {"pin", gui_cmd_pin, true},   ///< Inventaire d’un joueur
    {"sgt", gui_cmd_sgt, false},  ///< Obtenir l’unité de temps
    {"sst", gui_cmd_sst, true},   ///< Modifier l’unité de temps
    {NULL, NULL, false}           ///< Fin du tableau
};

/**
 * @brief Recherche le handler correspondant à une commande GUI.
 *
 * @param command Le nom de la commande.
 * @return Un pointeur vers le handler s’il est trouvé, NULL sinon.
 */
static const gui_handler_t *find_gui_handler(const char *command)
{
    if (!command) {
        return NULL;
    }
    for (int i = 0; GUI_HANDLERS[i].command; i++) {
        if (strcmp(GUI_HANDLERS[i].command, command) == 0) {
            return &GUI_HANDLERS[i];
        }
    }
    return NULL;
}

/**
 * @brief Valide les arguments d’une commande en fonction du handler.
 *
 * @param handler Le handler associé à la commande.
 * @param args Les arguments reçus.
 * @return true si les arguments sont valides, false sinon.
 */
static bool validate_gui_command(const gui_handler_t *handler, char **args)
{
    if (!handler->need_args) {
        return true;
    }
    if (!args || !args[1]) {
        printf("DEBUG: GUI command needs arguments but none provided\n");
        return false;
    }
    return true;
}

/**
 * @brief Exécute une commande GUI via son handler.
 *
 * @param server Le serveur principal.
 * @param client Le client GUI concerné.
 * @param handler Le handler de la commande.
 * @param args Les arguments.
 */
static void execute_gui_command(server_t *server, client_t *client,
    const gui_handler_t *handler, char **args)
{
    printf("DEBUG: Executing GUI command '%s'\n", handler->command);
    handler->handler(server, client, args);
}

/**
 * @brief Vérifie que les arguments de la commande sont valides.
 *
 * @param args Tableau des arguments.
 * @param client Le client GUI.
 * @return true si l’argument principal est présent, false sinon.
 */
static bool process_gui_args(char **args, client_t *client)
{
    if (!args || !args[0]) {
        gui_send_unknown_command(client);
        return false;
    }
    printf("DEBUG: GUI received command: '%s'\n", args[0]);
    return true;
}

/**
 * @brief Vérifie si le handler est valide et si les paramètres le sont aussi.
 *
 * @param handler Le handler trouvé.
 * @param args Les arguments de la commande.
 * @param client Le client concerné.
 * @return true si tout est valide, false sinon.
 */
static bool process_gui_handler(const gui_handler_t *handler, char **args,
    client_t *client)
{
    if (!handler) {
        printf("DEBUG: Unknown GUI command: '%s'\n", args[0]);
        gui_send_unknown_command(client);
        return false;
    }
    if (!validate_gui_command(handler, args)) {
        gui_send_bad_parameters(client);
        return false;
    }
    return true;
}

/**
 * @brief Fonction principale appelée lorsqu’un client graphique envoie une commande.
 *
 * Cette fonction parse, valide, recherche et exécute la commande.
 *
 * @param server Le serveur principal.
 * @param client Le client graphique.
 * @param line La commande reçue en texte brut.
 */
void gui_handle_command(server_t *server, client_t *client, const char *line)
{
    char **args = str_split(line, ' ');
    const gui_handler_t *handler = NULL;

    if (!process_gui_args(args, client)) {
        free_array(args);
        return;
    }
    handler = find_gui_handler(args[0]);
    if (!process_gui_handler(handler, args, client)) {
        free_array(args);
        return;
    }
    execute_gui_command(server, client, handler, args);
    free_array(args);
}

/**
 * @brief Envoie un message d'erreur pour commande inconnue (GUI).
 *
 * Format : `suc\n`
 *
 * @param client Le client graphique.
 */
void gui_send_unknown_command(client_t *client)
{
    network_send(client, "suc\n");
}

/**
 * @brief Envoie un message d'erreur pour mauvais paramètres (GUI).
 *
 * Format : `sbp\n`
 *
 * @param client Le client graphique.
 */
void gui_send_bad_parameters(client_t *client)
{
    network_send(client, "sbp\n");
}
