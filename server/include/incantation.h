/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation
*/

#ifndef INCANTATION_H_
    #define INCANTATION_H_

    #include "server.h"

    #define MAX_LEVEL 8
    #define MAX_PARTICIPANTS 16

typedef struct elevation_req_s {
    int level;
    int nb_players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} elevation_req_t;

typedef struct incantation_ctx_s {
    player_t **participants;
    int nb_participants;
    int level;
    int x;
    int y;
} incantation_ctx_t;

// incantation_validation.c
bool validate_incantation_player(client_t *client);
bool check_incantation_conditions(server_t *server, player_t *player,
    incantation_ctx_t *ctx);

// incantation_requirements.c
const elevation_req_t *get_elevation_requirements(int level);
bool check_player_requirements(server_t *server, player_t *player,
    incantation_ctx_t *ctx);
bool check_resource_requirements(server_t *server, player_t *player);

// incantation_execution.c
void execute_incantation_ritual(server_t *server, player_t *player,
    incantation_ctx_t *ctx);

// incantation_game_end.c
void check_game_end_condition(server_t *server);

#endif /* !INCANTATION_H_ */
