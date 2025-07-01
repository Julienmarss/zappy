/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_utils
*/

/**
 * @file commands_utils.c
 * @brief Fournit des utilitaires pour la gestion des ressources dans les commandes.
 */

#include "server.h"

/**
 * @brief Tableau de noms de ressources disponibles.
 */
static const char *RESOURCE_NAMES[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

/**
 * @brief Récupère l'indice d'une ressource à partir de son nom.
 *
 * @param resource_name Le nom de la ressource (ex: "food").
 * @return L'indice correspondant (0 à NB_RESOURCES - 1), ou -1 si le nom est invalide.
 */
int get_resource_index_by_name(const char *resource_name)
{
    if (!resource_name)
        return -1;
    for (int i = 0; i < NB_RESOURCES; i++) {
        if (strcmp(RESOURCE_NAMES[i], resource_name) == 0)
            return i;
    }
    return -1;
}

/**
 * @brief Vérifie si un nom de ressource est valide.
 *
 * @param resource_name Le nom de la ressource à vérifier.
 * @return true si le nom est reconnu, false sinon.
 */
bool is_valid_resource_name(const char *resource_name)
{
    return get_resource_index_by_name(resource_name) != -1;
}

/**
 * @brief Récupère le nom d'une ressource à partir de son indice.
 *
 * @param index L'indice de la ressource (0 à NB_RESOURCES - 1).
 * @return Le nom de la ressource, ou NULL si l’indice est invalide.
 */
const char *get_resource_name_by_index(int index)
{
    if (index < 0 || index >= NB_RESOURCES)
        return NULL;
    return RESOURCE_NAMES[index];
}
