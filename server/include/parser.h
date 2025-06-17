/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Main server header
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <poll.h>
    #include <errno.h>
    #include <stdbool.h>
    #include <time.h>

    #define MAX_CLIENTS 1024
    #define BUFFER_SIZE 4096
    #define MAX_TEAMS 10
    #define MAX_CMD_QUEUE 10
    #define DEFAULT_PORT 4242
    #define DEFAULT_WIDTH 10
    #define DEFAULT_HEIGHT 10
    #define DEFAULT_FREQ 100

enum resource_e {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    NB_RESOURCES
};

enum orientation_e {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

typedef enum {
    CLIENT_UNKNOWN,
    CLIENT_PLAYER,
    CLIENT_GRAPHIC
} client_type_t;

typedef struct s_server server_t;
typedef struct s_client client_t;
typedef struct s_player player_t;
typedef struct s_game game_t;
typedef struct s_team team_t;
typedef struct s_egg egg_t;
typedef struct s_tile tile_t;
typedef struct s_command command_t;
typedef struct s_cmd_info cmd_info_t;
typedef struct s_game_params game_params_t;

struct s_game_params {
    int width;
    int height;
    char **teams;
    int nb_clients;
    int freq;
};

struct s_command {
    char *cmd;
    int time_limit;
    struct s_command *next;
};

struct s_cmd_info {
    const char *name;
    void (*func)(server_t *, client_t *, char **);
    int time;
};

struct s_client {
    int fd;
    client_type_t type;
    char read_buffer[BUFFER_SIZE];
    int read_pos;
    char write_buffer[BUFFER_SIZE];
    int write_pos;
    player_t *player;
    command_t *cmd_queue;
    int cmd_count;
    struct s_client *next;
};

struct s_player {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    int inventory[NB_RESOURCES];
    int food_timer;
    team_t *team;
    client_t *client;
    bool is_incanting;
    int action_time;
};

struct s_egg {
    int id;
    int x;
    int y;
    team_t *team;
    struct s_egg *next;
};

struct s_team {
    char *name;
    int max_clients;
    int connected_clients;
    egg_t *eggs;
    struct s_team *next;
};

struct s_tile {
    int resources[NB_RESOURCES];
    player_t **players;
    int player_count;
    egg_t *eggs;
};

struct s_game {
    int width;
    int height;
    tile_t **map;
    team_t *teams;
    int time_unit;
    int player_id_counter;
    int egg_id_counter;
    long last_resource_spawn;
};

struct s_server {
    int port;
    int socket_fd;
    struct pollfd *poll_fds;
    int poll_count;
    client_t *clients;
    game_t *game;
    long current_time;
};

// server.c
server_t *server_create(int port, int width, int height, char **teams,
    int nb_clients, int freq);
void server_destroy(server_t *server);
int server_run(server_t *server);

// server_accept.c
void server_accept_client(server_t *server);
void server_disconnect_client(server_t *server, client_t *client);

// network.c
int network_send(client_t *client, const char *msg);
int network_receive(server_t *server, client_t *client);
void network_process_commands(server_t *server, client_t *client);

// network_handler.c
void network_handle_client_line(server_t *server, client_t *client,
    const char *line);

// network_client.c
void network_handle_new_client(server_t *server, client_t *client,
    const char *team_name);

// network_player.c
void network_reject_connection(server_t *server, client_t *client);
void network_handle_player_connection(server_t *server, client_t *client,
    team_t *team);
void network_send_connection_info(client_t *client, team_t *team,
    int width, int height);

// network_gui.c
void network_handle_graphic_connection(server_t *server, client_t *client);

// game.c
void game_destroy(game_t *game);
void game_update(server_t *server);
tile_t *game_get_tile(game_t *game, int x, int y);
void free_map(game_t *game);

// game_creation.c
game_t *game_create(game_params_t *params);
tile_t **create_map(int width, int height);

// game_teams.c
void init_teams(game_t *game, char **teams, int nb_clients);

// game_resources.c
void game_spawn_resources(game_t *game);

// player.c
player_t *player_create(team_t *team, int x, int y);
void player_destroy(player_t *player);
void player_update(server_t *server, player_t *player);
void player_die(server_t *server, player_t *player);
void player_add_to_tile(server_t *server, player_t *player);

// commands_move.c
void cmd_forward(server_t *server, client_t *client, char **args);
void cmd_right(server_t *server, client_t *client, char **args);
void cmd_left(server_t *server, client_t *client, char **args);

// commands_info.c
void cmd_look(server_t *server, client_t *client, char **args);
void cmd_inventory(server_t *server, client_t *client, char **args);
void cmd_broadcast(server_t *server, client_t *client, char **args);
void cmd_connect_nbr(server_t *server, client_t *client, char **args);

// commands_action.c
void cmd_fork(server_t *server, client_t *client, char **args);
void cmd_eject(server_t *server, client_t *client, char **args);
void cmd_take(server_t *server, client_t *client, char **args);
void cmd_set(server_t *server, client_t *client, char **args);
void cmd_incantation(server_t *server, client_t *client, char **args);

// gui_protocol.c
void gui_send_map_size(server_t *server, client_t *client);
void gui_send_tile_content(server_t *server, client_t *client, int x, int y);
void gui_send_teams(server_t *server, client_t *client);
void gui_send_player_position(server_t *server, client_t *client,
    player_t *player);
void gui_broadcast_player_action(server_t *server, player_t *player,
    const char *action);

// utils.c
void free_array(char **array);
long get_time_microseconds(void);
char *str_concat(const char *s1, const char *s2);
char *int_to_str(int n);

// str_utils.c
char **str_split(const char *str, char delim);

#endif /* !SERVER_H_ */
