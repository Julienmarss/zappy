/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** str_utils
*/

/**
 * @file str_utils.c
 * @brief Fournit des fonctions utilitaires pour manipuler les chaînes de caractères.
 */

#include "server.h"

/**
 * @brief Compte le nombre de mots dans une chaîne, séparés par un délimiteur.
 * 
 * @param str Chaîne à analyser.
 * @param delim Caractère délimiteur.
 * @return Nombre de mots détectés.
 */
static int count_words(const char *str, char delim)
{
    int count = 0;
    bool in_word = false;

    for (int i = 0; str[i]; i++) {
        if (str[i] != delim && !in_word) {
            count++;
            in_word = true;
        }
        if (str[i] == delim)
            in_word = false;
    }
    return count;
}

/**
 * @brief Extrait un mot de la chaîne à partir d’un index, jusqu’au délimiteur.
 * 
 * @param str Chaîne source.
 * @param idx Index de départ (sera modifié).
 * @param delim Délimiteur.
 * @return Mot extrait (chaîne allouée dynamiquement).
 */
static char *extract_word(const char *str, int *idx, char delim)
{
    int start = *idx;
    int len = 0;
    char *word = NULL;

    while (str[*idx] && str[*idx] != delim) {
        (*idx)++;
        len++;
    }
    if (len == 0)
        return NULL;
    word = malloc(len + 1);
    if (!word)
        return NULL;
    strncpy(word, str + start, len);
    word[len] = '\0';
    return word;
}

/**
 * @brief Alloue un tableau de chaînes pour le résultat du split.
 * 
 * @param words Nombre de mots à contenir.
 * @return Tableau alloué (terminé par NULL).
 */
static char **allocate_result(int words)
{
    return calloc(words + 1, sizeof(char *));
}

/**
 * @brief Ignore un caractère délimiteur dans la chaîne.
 * 
 * @param str Chaîne source.
 * @param idx Index courant (sera modifié).
 * @param delim Caractère délimiteur.
 */
static void skip_delimiter(const char *str, int *idx, char delim)
{
    if (str[*idx] == delim)
        (*idx)++;
}

/**
 * @brief Gère un caractère courant dans la logique du split.
 * 
 * @param str Chaîne source.
 * @param ctx Contexte du split.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
static int process_character(const char *str, split_ctx_t *ctx)
{
    if (str[*ctx->idx] != ctx->delim) {
        ctx->result[*ctx->word_idx] = extract_word(str, ctx->idx, ctx->delim);
        if (!ctx->result[*ctx->word_idx]) {
            free_array(ctx->result);
            return -1;
        }
        (*ctx->word_idx)++;
    } else {
        skip_delimiter(str, ctx->idx, ctx->delim);
    }
    return 0;
}

/**
 * @brief Découpe une chaîne en mots selon un délimiteur.
 * 
 * @param str Chaîne source.
 * @param delim Caractère délimiteur.
 * @param words Nombre de mots à extraire.
 * @return Tableau de mots alloué dynamiquement.
 */
static char **split_words(const char *str, char delim, int words)
{
    char **result = allocate_result(words);
    int idx = 0;
    int word_idx = 0;
    split_ctx_t ctx = {result, &idx, &word_idx, delim};

    if (!result)
        return NULL;
    while (str[idx]) {
        if (process_character(str, &ctx) == -1)
            return NULL;
    }
    result[word_idx] = NULL;
    return result;
}

/**
 * @brief Vérifie si un caractère est un espace blanc.
 * 
 * @param c Caractère à vérifier.
 * @return true si c'est un espace blanc, false sinon.
 */
static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/**
 * @brief Découpe une chaîne en mots en fonction d’un délimiteur.
 * 
 * @param str Chaîne source.
 * @param delim Caractère délimiteur.
 * @return Tableau de chaînes alloué dynamiquement, ou NULL en cas d'erreur.
 */
char **str_split(const char *str, char delim)
{
    int words = count_words(str, delim);

    if (words == 0)
        return NULL;
    return split_words(str, delim, words);
}

/**
 * @brief Supprime les espaces blancs à la fin d'une chaîne.
 * 
 * @param str Chaîne à modifier.
 * @return Pointeur vers la même chaîne (modifiée).
 */
char *str_trim(char *str)
{
    int len = 0;
    int end = 0;

    if (!str)
        return NULL;
    len = strlen(str);
    if (len == 0)
        return str;
    end = len - 1;
    while (end >= 0 && is_whitespace(str[end]))
        end--;
    str[end + 1] = '\0';
    return str;
}
