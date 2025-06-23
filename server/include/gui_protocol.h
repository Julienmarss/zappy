/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI protocol header file
*/

#ifndef GUI_H_
    #define GUI_H_

    #include "server.h"

    #define MAX_GUI_RESPONSE 1024

typedef struct s_gui_handler {
    const char *command;
    void (*handler)(server_t *server, client_t *client, char **args);
    bool need_args;
} gui_handler_t;

typedef enum action_type_e {
    ACTION_MOVE,
    ACTION_TURN,
    ACTION_TAKE,
    ACTION_SET,
    ACTION_EJECT,
    ACTION_INCANTATION,
    ACTION_UNKNOWN
} action_type_t;

// gui_protocol.c
void gui_init_handlers(void);
void gui_handle_command(server_t *server, client_t *client, const char *line);
void gui_send_unknown_command(client_t *client);
void gui_send_bad_parameters(client_t *client);

// gui_map_commands.c
void gui_cmd_msz(server_t *server, client_t *client, char **args);
void gui_cmd_bct(server_t *server, client_t *client, char **args);
void gui_cmd_mct(server_t *server, client_t *client, char **args);

// gui_team_commands.c
void gui_cmd_tna(server_t *server, client_t *client, char **args);

// gui_player_commands.c
void gui_cmd_ppo(server_t *server, client_t *client, char **args);
void gui_cmd_plv(server_t *server, client_t *client, char **args);
void gui_cmd_pin(server_t *server, client_t *client, char **args);

// gui_time_commands.c
void gui_cmd_sgt(server_t *server, client_t *client, char **args);
void gui_cmd_sst(server_t *server, client_t *client, char **args);

// gui_broadcast.c
void gui_broadcast_new_player(server_t *server, player_t *player);
void gui_broadcast_player_position(server_t *server, player_t *player);
void gui_broadcast_player_level(server_t *server, player_t *player);
void gui_broadcast_player_inventory(server_t *server, player_t *player);
void gui_broadcast_player_expulsion(server_t *server, player_t *player);
void gui_broadcast_player_broadcast(server_t *server, player_t *player,
    const char *message);
void gui_broadcast_incantation_start(server_t *server, int x, int y, int level,
    player_t **players, int nb_players);
void gui_broadcast_incantation_end(server_t *server, int x, int y,
    bool result);
void gui_broadcast_player_fork(server_t *server, player_t *player);
void gui_broadcast_resource_drop(server_t *server, player_t *player,
    int resource);
void gui_broadcast_resource_collect(server_t *server, player_t *player,
    int resource);
void gui_broadcast_player_death(server_t *server, player_t *player);
void gui_broadcast_egg_laid(server_t *server, egg_t *egg, player_t *player);
void gui_broadcast_egg_connection(server_t *server, egg_t *egg);
void gui_broadcast_egg_death(server_t *server, egg_t *egg);
void gui_broadcast_game_end(server_t *server, const char *team_name);
void gui_broadcast_server_message(server_t *server, const char *message);

// gui_utils.c
void gui_send_to_all_graphic_clients(server_t *server, const char *message);
char *gui_format_player_id(int player_id);
char *gui_format_egg_id(int egg_id);
bool gui_is_valid_coordinates(server_t *server, int x, int y);
bool gui_is_valid_player_id(server_t *server, int player_id);

void eject_single_player(server_t *server, player_t *ejector,
    player_t *target);
void remove_player_from_old_tile(server_t *server, player_t *player);
void add_player_to_new_tile(server_t *server, player_t *player,
    int new_x, int new_y);
void destroy_eggs_on_tile(server_t *server, int x, int y);

void gui_broadcast_player_action(server_t *server, player_t *player,
    const char *action_type);

#endif /* !GUI_H_ */
