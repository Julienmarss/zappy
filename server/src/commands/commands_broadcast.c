/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_broadcast - Version finale corrigée
*/

/**
 * @file commands_broadcast.c
 * @brief Gère la commande de broadcast d’un joueur vers les autres, en calculant la direction du son.
 */

#include "server.h"
#include "gui_protocol.h"

/**
 * @brief Calcule la distance la plus courte sur l’axe X (avec torus horizontal).
 *
 * @param dx Distance brute entre deux positions X.
 * @param map_w Largeur de la carte.
 * @return Distance corrigée avec wrap-around.
 */
static int get_delta_x(int dx, int map_w)
{
    if (dx > map_w / 2)
        dx -= map_w;
    if (dx < -(map_w / 2))
        dx += map_w;
    return dx;
}

/**
 * @brief Calcule la distance la plus courte sur l’axe Y (avec torus vertical).
 *
 * @param dy Distance brute entre deux positions Y.
 * @param map_h Hauteur de la carte.
 * @return Distance corrigée avec wrap-around.
 */
static int get_delta_y(int dy, int map_h)
{
    if (dy > map_h / 2)
        dy -= map_h;
    if (dy < -(map_h / 2))
        dy += map_h;
    return dy;
}

/**
 * @brief Définit une direction de base si aucune n'a encore été trouvée.
 *
 * @param direction Pointeur vers la direction à modifier.
 * @param check Pointeur vers le flag de validation.
 * @param value Valeur directionnelle à affecter.
 */
static void set_base_direction(int *direction, bool *check, int value)
{
    *direction = value;
    *check = true;
}

/**
 * @brief Calcule la direction de base en fonction de dx et dy.
 *
 * @param dx Delta X (après wrap).
 * @param dy Delta Y (après wrap).
 * @param base_direction Valeur initiale à utiliser (0).
 * @return Direction de base entre 1 et 8, ou 0 si non applicable.
 */
static int get_base_direction(int dx, int dy, int base_direction)
{
    bool check = false;

    if (dx == 0 && dy < 0)
        set_base_direction(&base_direction, &check, 1);
    if (dx < 0 && dy < 0)
        set_base_direction(&base_direction, &check, 2);
    if (dx < 0 && dy == 0)
        set_base_direction(&base_direction, &check, 3);
    if (dx < 0 && dy > 0)
        set_base_direction(&base_direction, &check, 4);
    if (dx == 0 && dy > 0)
        set_base_direction(&base_direction, &check, 5);
    if (dx > 0 && dy > 0)
        set_base_direction(&base_direction, &check, 6);
    if (dx > 0 && dy == 0)
        set_base_direction(&base_direction, &check, 7);
    if (dx > 0 && dy < 0)
        set_base_direction(&base_direction, &check, 8);
    base_direction = !check ? 0 : base_direction;
    return base_direction;
}

/**
 * @brief Convertit la direction de base en direction relative à l’orientation du joueur.
 *
 * @param base_direction Direction absolue de l'émetteur par rapport au récepteur.
 * @param direction Orientation du récepteur (1 = Nord, 2 = Est, etc.).
 * @param res Valeur de base à retourner si aucune correspondance.
 * @return Direction finale reçue par le joueur (entre 1 et 8).
 */
static int set_res_from_direction(int base_direction, int direction, int res)
{
    if (base_direction == 0)
        return 0;
    if (direction == 1)
        return base_direction;
    if (direction == 2) {
        res = base_direction + 2;
        if (res > 8)
            res -= 8;
    }
    if (direction == 3) {
        res = base_direction + 4;
        if (res > 8)
            res -= 8;
    }
    if (direction == 4) {
        res = base_direction - 2;
        if (res < 1)
            res += 8;
    }
    return res;
}

/**
 * @brief Calcule la direction d’un message broadcast pour un joueur récepteur.
 *
 * @param server Le serveur contenant la carte.
 * @param sender Le joueur émetteur.
 * @param receiver Le joueur récepteur.
 * @return Direction (1-8) perçue par le récepteur, 0 si le même joueur.
 */
static int calculate_sound_direction_for_players(server_t *server,
    player_t *sender, player_t *receiver)
{
    int map_width = server->game->width;
    int map_height = server->game->height;
    int dx = sender->x - receiver->x;
    int dy = sender->y - receiver->y;
    int direction = receiver->orientation;
    int base_direction;
    int res;

    dx = get_delta_x(dx, map_width);
    dy = get_delta_y(dy, map_height);
    base_direction = get_base_direction(dx, dy, 0);
    res = set_res_from_direction(base_direction, direction, 0);
    return res;
}

/**
 * @brief Reconstruit le message complet à partir des arguments de la commande.
 *
 * @param args Tableau de chaînes représentant les arguments.
 * @return Pointeur vers le message reconstruit.
 */
char *reconstruct_broadcast_message(char **args)
{
    static char full_message[1024] = "";
    int i = 1;

    memset(full_message, 0, sizeof(full_message));
    while (args[i]) {
        if (i > 1)
            strcat(full_message, " ");
        strcat(full_message, args[i]);
        i++;
    }
    return full_message;
}

/**
 * @brief Envoie le message broadcast à tous les joueurs sauf l’émetteur, avec direction.
 *
 * @param server Le serveur contenant les clients.
 * @param emitter Le joueur qui émet le message.
 * @param message Le message à diffuser.
 */
void send_broadcast_to_listeners(server_t *server,
    player_t *emitter, char *message)
{
    client_t *current = server->clients;
    char response[1024];
    int direction = 0;

    while (current) {
        if (current->type == CLIENT_PLAYER && current->player &&
            current->player != emitter) {
            direction = calculate_sound_direction_for_players(server,
                emitter, current->player);
            snprintf(response, sizeof(response), "message %d, %s\n",
                direction, message);
            network_send(current, response);
        }
        current = current->next;
    }
}

/**
 * @brief Commande principale : envoie un message broadcast aux autres joueurs.
 *
 * @param server Le serveur principal.
 * @param client Le client ayant envoyé la commande.
 * @param args Les arguments de la commande (args[1] = message).
 */
void cmd_broadcast(server_t *server, client_t *client, char **args)
{
    player_t *emitter = client->player;
    char *message = NULL;

    if (!emitter) {
        printf("DEBUG: Broadcast failed - no player\n");
        network_send(client, "ko\n");
        return;
    }
    if (!args[1]) {
        printf("DEBUG: Broadcast failed - no message\n");
        network_send(client, "ko\n");
        return;
    }
    message = reconstruct_broadcast_message(args);
    send_broadcast_to_listeners(server, emitter, message);
    network_send(client, "ok\n");
    gui_broadcast_player_broadcast(server, emitter, message);
}
