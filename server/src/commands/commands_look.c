/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** commands_look
*/

/**
 * @file commands_look.c
 * @brief Implémentation de la commande `Look` pour afficher la vision du joueur.
 */

#include "server.h"
#include "vision.h"

/**
 * @brief Valide la requête `Look` pour s'assurer que le client possède un joueur.
 *
 * @param client Le client ayant envoyé la commande.
 * @return true si la requête est valide, false sinon (avec envoi de "ko").
 */
static bool validate_look_request(client_t *client)
{
    if (!client->player) {
        network_send(client, "ko\n");
        return false;
    }
    return true;
}

/**
 * @brief Finalise la réponse `Look` en ajoutant un saut de ligne et l'envoie au client.
 *
 * @param client Le client cible.
 * @param buffer La réponse formatée.
 */
static void finalize_look_response(client_t *client, char *buffer)
{
    strcat(buffer, "\n");
    network_send(client, buffer);
}

/**
 * @brief Initialise la structure de formatage de la vision.
 *
 * @param server Le serveur principal.
 * @param client Le client ayant envoyé la commande.
 * @param vision_data Données calculées de la vision.
 * @param format Structure de formatage à remplir.
 */
static void create_vision_format(server_t *server, client_t *client,
    vision_data_t *vision_data, vision_format_t *format)
{
    format->server = server;
    format->player = client->player;
    format->data = vision_data;
}

/**
 * @brief Commande `Look` : génère la vision du joueur autour de lui selon son niveau.
 *
 * La réponse est construite à partir de la fonction `vision_format_response`,
 * en utilisant un pattern de vision et les ressources visibles à chaque tuile.
 *
 * @param server Le serveur.
 * @param client Le client ayant envoyé la commande.
 * @param args Arguments passés (ignorés ici).
 */
void cmd_look(server_t *server, client_t *client, char **args)
{
    char buffer[16384] = {0};
    vision_data_t *vision_data = NULL;
    vision_format_t format = {0};

    (void)args;
    if (!validate_look_request(client))
        return;
    vision_data = vision_create_data(client->player->level);
    if (!vision_data) {
        network_send(client, "ko\n");
        return;
    }
    vision_calculate_tiles(vision_data);
    create_vision_format(server, client, vision_data, &format);
    vision_format_response(&format, buffer);
    vision_destroy_data(vision_data);
    finalize_look_response(client, buffer);
}
