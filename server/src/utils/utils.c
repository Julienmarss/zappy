/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Utility functions
*/

#include "server.h"
#include <sys/time.h>

long get_time_microseconds(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void free_array(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i]; i++)
        free(array[i]);
    free(array);
}

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

char *int_to_str(int n)
{
    char buffer[32];
    
    snprintf(buffer, sizeof(buffer), "%d", n);
    return strdup(buffer);
}
