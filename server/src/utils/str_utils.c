/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** str_utils
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
        }
        if (str[i] == delim)
            in_word = false;
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

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

char **str_split(const char *str, char delim)
{
    int words = count_words(str, delim);

    if (words == 0)
        return NULL;
    return split_words(str, delim, words);
}

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
