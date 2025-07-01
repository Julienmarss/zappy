/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_run
*/

/**
 * @file server_run.c
 * @brief Contient la boucle principale du serveur Zappy, qui gère les événements réseau et les mises à jour du jeu.
 */

#include "server.h"

/**
 * @brief Calcule le timeout à passer à `poll`, basé sur l’unité de temps du jeu.
 *
 * @param server Pointeur vers la structure du serveur.
 * @return Timeout en millisecondes.
 */
static int calculate_timeout(server_t *server)
{
    long current = get_time_microseconds();
    long elapsed = current - server->current_time;
    long time_unit_us = 1000000 / server->game->time_unit;
    int timeout = (time_unit_us - elapsed) / 1000;

    return (timeout < 0) ? 0 : timeout;
}

/**
 * @brief Détermine si une mise à jour de l’état du jeu doit être effectuée.
 *
 * @param server Pointeur vers la structure du serveur.
 * @return true si le jeu doit être mis à jour, false sinon.
 */
static bool should_update_game(server_t *server)
{
    long current = get_time_microseconds();
    long time_unit_us = 1000000 / server->game->time_unit;

    if (current - server->current_time >= time_unit_us) {
        server->current_time = current;
        return true;
    }
    return false;
}

/**
 * @brief Gère les erreurs de retour de `poll`.
 *
 * @param ret Valeur de retour de poll().
 * @return 0 si tout va bien, 1 pour une interruption ignorée (EINTR),
 *         84 pour une erreur fatale.
 */
static int handle_poll_error(int ret)
{
    if (ret >= 0)
        return 0;
    if (errno == EINTR)
        return 1;
    perror("poll");
    return 84;
}

/**
 * @brief Boucle principale du serveur. Gère le polling des clients et les mises à jour du jeu.
 *
 * @param server Pointeur vers la structure du serveur.
 * @return 0 en cas de succès, 84 en cas d'erreur.
 */
int server_run(server_t *server)
{
    int ret = 0;
    int timeout = 0;
    int error_code = 0;

    while (1) {
        timeout = calculate_timeout(server);
        ret = poll(server->poll_fds, server->poll_count, timeout);
        error_code = handle_poll_error(ret);
        if (error_code == 84)
            return 84;
        if (error_code == 1)
            continue;
        handle_poll_events(server, ret);
        if (should_update_game(server))
            game_update(server);
    }
    return 0;
}
