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

typedef struct s_server t_server;
typedef struct s_client t_client;
typedef struct s_player t_player;
typedef struct s_game t_game;
typedef struct s_team t_team;
typedef struct s_egg t_egg;
typedef struct s_tile t_tile;
typedef struct s_command t_command;

struct s_command {
    char *cmd;
    int time_limit;
    struct s_command *next;
};

struct s_client {
    int fd;
    client_type_t type;
    char read_buffer[BUFFER_SIZE];
    int read_pos;
    char write_buffer[BUFFER_SIZE];
    int write_pos;
    t_player *player;
    t_command *cmd_queue;
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
    t_team *team;
    t_client *client;
    bool is_incanting;
    int action_time;
};

struct s_egg {
    int id;
    int x;
    int y;
    t_team *team;
    struct s_egg *next;
};

struct s_team {
    char *name;
    int max_clients;
    int connected_clients;
    t_egg *eggs;
    struct s_team *next;
};

struct s_tile {
    int resources[NB_RESOURCES];
    t_player **players;
    int player_count;
    t_egg *eggs;
};

struct s_game {
    int width;
    int height;
    t_tile **map;
    t_team *teams;
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
    t_client *clients;
    t_game *game;
    long current_time;
};

// server.c
t_server *server_create(int port, int width, int height, char **teams,
    int nb_clients, int freq);
void server_destroy(t_server *server);
int server_run(t_server *server);

// server_accept.c
void server_accept_client(t_server *server);
void server_disconnect_client(t_server *server, t_client *client);

// network.c
int network_send(t_client *client, const char *msg);
int network_receive(t_server *server, t_client *client);
void network_process_commands(t_server *server, t_client *client);

// network_client.c
void network_handle_new_client(t_server *server, t_client *client,
    const char *team_name);

// game.c
t_game *game_create(int width, int height, char **teams, int nb_clients,
    int freq);
void game_destroy(t_game *game);
void game_update(t_server *server);
void game_spawn_resources(t_game *game);
t_tile *game_get_tile(t_game *game, int x, int y);

// game_teams.c
void init_teams(t_game *game, char **teams, int nb_clients);

// game_map.c  
t_tile **create_map(int width, int height);
void free_map(t_game *game);

// player.c
t_player *player_create(t_team *team, int x, int y);
void player_destroy(t_player *player);
void player_update(t_server *server, t_player *player);
void player_die(t_server *server, t_player *player);

// commands_move.c
void cmd_forward(t_server *server, t_client *client, char **args);
void cmd_right(t_server *server, t_client *client, char **args);
void cmd_left(t_server *server, t_client *client, char **args);

// commands_info.c
void cmd_look(t_server *server, t_client *client, char **args);
void cmd_inventory(t_server *server, t_client *client, char **args);
void cmd_broadcast(t_server *server, t_client *client, char **args);
void cmd_connect_nbr(t_server *server, t_client *client, char **args);

// commands_action.c
void cmd_fork(t_server *server, t_client *client, char **args);
void cmd_eject(t_server *server, t_client *client, char **args);
void cmd_take(t_server *server, t_client *client, char **args);
void cmd_set(t_server *server, t_client *client, char **args);
void cmd_incantation(t_server *server, t_client *client, char **args);

// gui_protocol.c
void gui_send_map_size(t_server *server, t_client *client);
void gui_send_tile_content(t_server *server, t_client *client, int x, int y);
void gui_send_teams(t_server *server, t_client *client);
void gui_send_player_position(t_server *server, t_client *client,
    t_player *player);
void gui_broadcast_player_action(t_server *server, t_player *player,
    const char *action);

// utils.c
void free_array(char **array);
long get_time_microseconds(void);
char *str_concat(const char *s1, const char *s2);
char *int_to_str(int n);

// str_utils.c
char **str_split(const char *str, char delim);

#endif /* !SERVER_H_ */