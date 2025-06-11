/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** String utilities
*/

#include "server.h"

static int count_words(const char *str, char delim)
{
    int count = 0;
    bool in_word = false;

    for (int i = 0; str[i]; i++) {
        if (str[i] != delim && !in_word) {
            count++;
            in_word = true;
        } else if (str[i] == delim) {
            in_word = false;
        }
    }
    return count;
}

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

static char **allocate_result(int words)
{
    char **result = calloc(words + 1, sizeof(char *));

    return result;
}

static void skip_delimiter(const char *str, int *idx, char delim)
{
    if (str[*idx] == delim)
        (*idx)++;
}

char **str_split(const char *str, char delim)
{
    int words = count_words(str, delim);
    char **result = NULL;
    int idx = 0;
    int word_idx = 0;

    if (words == 0)
        return NULL;
    result = allocate_result(words);
    if (!result)
        return NULL;
    while (str[idx]) {
        if (str[idx] != delim) {
            result[word_idx] = extract_word(str, &idx, delim);
            if (!result[word_idx]) {
                free_array(result);
                return NULL;
            }
            word_idx++;
        } else {
            skip_delimiter(str, &idx, delim);
        }
    }
    result[word_idx] = NULL;
    return result;
}
