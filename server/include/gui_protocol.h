/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_protocol
*/

#ifndef GUI_PROTOCOL_H_
    #define GUI_PROTOCOL_H_

    #include "server.h"

    #define MAX_GUI_RESPONSE 1024

typedef struct s_gui_handler {
    const char *command;
    void (*handler)(server_t *server, client_t *client, char **args);
    bool need_args;
} gui_handler_t;

// gui_protocol.c
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

// gui_utils.c
void gui_send_to_all_graphic_clients(server_t *server, const char *message);
char *gui_format_player_id(int player_id);
char *gui_format_egg_id(int egg_id);
bool gui_is_valid_coordinates(server_t *server, int x, int y);
bool gui_is_valid_player_id(server_t *server, int player_id);

// gui_broadcast_player.c
void gui_broadcast_new_player(server_t *server, player_t *player);
void gui_broadcast_player_position(server_t *server, player_t *player);
void gui_broadcast_player_level(server_t *server, player_t *player);
void gui_broadcast_player_inventory(server_t *server, player_t *player);
void gui_broadcast_player_death(server_t *server, player_t *player);

// gui_broadcast_action.c
void gui_broadcast_player_expulsion(server_t *server, player_t *player);
void gui_broadcast_player_fork(server_t *server, player_t *player);
void gui_broadcast_player_broadcast(server_t *server, player_t *player,
    const char *broadcast_message);

// gui_broadcast_resource.c
void gui_broadcast_resource_drop(server_t *server, player_t *player,
    int resource);
void gui_broadcast_resource_collect(server_t *server, player_t *player,
    int resource);

// gui_broadcast_egg.c
void gui_broadcast_egg_laid(server_t *server, egg_t *egg, player_t *player);
void gui_broadcast_egg_connection(server_t *server, egg_t *egg);
void gui_broadcast_egg_death(server_t *server, egg_t *egg);

// gui_broadcast_incantation.c
void gui_broadcast_incantation_start(server_t *server, int x, int y, int level,
    player_t **players, int nb_players);
void gui_broadcast_incantation_end(server_t *server, int x, int y,
    bool result);

// gui_broadcast_game.c
void gui_broadcast_game_end(server_t *server, const char *team_name);
void gui_broadcast_server_message(server_t *server, const char *server_msg);

#endif /* !GUI_PROTOCOL_H_ */
