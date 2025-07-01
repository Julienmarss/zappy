/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** utils
*/

/**
 * @file utils.c
 * @brief Fonctions utilitaires générales utilisées dans le serveur Zappy.
 */

#include "server.h"
#include <sys/time.h>

/**
 * @brief Récupère le temps actuel en microsecondes.
 * 
 * @return Temps en microsecondes depuis l'époque Unix.
 */
long get_time_microseconds(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

/**
 * @brief Libère un tableau de chaînes de caractères alloué dynamiquement.
 * 
 * @param array Tableau à libérer (doit être NULL-terminé).
 */
void free_array(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i]; i++)
        free(array[i]);
    free(array);
}

/**
 * @brief Concatène deux chaînes de caractères en une seule.
 * 
 * @param s1 Première chaîne.
 * @param s2 Deuxième chaîne.
 * @return Chaîne concaténée allouée dynamiquement (ou NULL en cas d'erreur).
 */
char *str_concat(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);

    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * @brief Convertit un entier en chaîne de caractères.
 * 
 * @param n Entier à convertir.
 * @return Chaîne représentant l'entier (allouée dynamiquement).
 */
char *int_to_str(int n)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%d", n);
    return strdup(buffer);
}
