/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Commands utility functions
*/

#include "server.h"

static const char *RESOURCE_NAMES[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

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

bool is_valid_resource_name(const char *resource_name)
{
    return get_resource_index_by_name(resource_name) != -1;
}

const char *get_resource_name_by_index(int index)
{
    if (index < 0 || index >= NB_RESOURCES)
        return NULL;
    return RESOURCE_NAMES[index];
}
