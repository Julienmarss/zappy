/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network
*/

/**
 * @file network.c
 * @brief Implémente les fonctions de lecture/écriture réseau pour les clients du serveur Zappy.
 */

#include "server.h"

/**
 * @brief Envoie un message au client via son socket.
 *
 * Cette fonction copie le message dans le buffer d’écriture du client,
 * puis tente immédiatement de l’envoyer via un appel à `write`.
 *
 * @param client Le client destinataire.
 * @param msg Le message à envoyer (terminé par '\n').
 * @return Nombre d'octets écrits, ou -1 en cas d’erreur de buffer plein.
 */
int network_send(client_t *client, const char *msg)
{
    int len = strlen(msg);
    int remaining = BUFFER_SIZE - client->write_pos;
    int to_write = 0;
    int written = 0;

    if (remaining < len)
        return -1;
    memcpy(client->write_buffer + client->write_pos, msg, len);
    client->write_pos += len;
    to_write = client->write_pos;
    written = write(client->fd, client->write_buffer, to_write);
    if (written > 0) {
        memmove(client->write_buffer, client->write_buffer + written,
            client->write_pos - written);
        client->write_pos -= written;
    }
    return written;
}

/**
 * @brief Extrait une ligne terminée par `\n` du buffer de lecture du client.
 *
 * @param client Le client dont on veut extraire une ligne.
 * @param line Le buffer dans lequel stocker la ligne extraite.
 * @param max_len Taille maximale du buffer `line`.
 * @return 1 si une ligne a été extraite, 0 sinon.
 */
static int extract_line(client_t *client, char *line, int max_len)
{
    int i = 0;

    for (i = 0; i < client->read_pos && i < max_len - 1; i++) {
        if (client->read_buffer[i] == '\n') {
            memcpy(line, client->read_buffer, i);
            line[i] = '\0';
            memmove(client->read_buffer, client->read_buffer + i + 1,
                client->read_pos - i - 1);
            client->read_pos -= i + 1;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Gère les codes de retour de `read` pour la lecture réseau.
 *
 * @param bytes Nombre de bytes lus.
 * @return 1 si la lecture peut continuer, 0 si le client a fermé, -1 si erreur fatale.
 */
static int handle_read_result(int bytes)
{
    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 1;
        return -1;
    }
    if (bytes == 0)
        return 0;
    return bytes;
}

/**
 * @brief Reçoit les données d’un client et traite les lignes complètes.
 *
 * Les lignes extraites sont immédiatement transmises à `network_handle_client_line`
 * pour traitement. Si le socket est fermé ou en erreur, la fonction retourne 0 ou -1.
 *
 * @param server Le serveur.
 * @param client Le client concerné.
 * @return Nombre de bytes reçus, 0 si fermeture, -1 si erreur.
 */
int network_receive(server_t *server, client_t *client)
{
    int bytes = 0;
    char line[BUFFER_SIZE];

    bytes = read(client->fd, client->read_buffer + client->read_pos,
        BUFFER_SIZE - client->read_pos - 1);
    bytes = handle_read_result(bytes);
    if (bytes <= 0)
        return bytes;
    client->read_pos += bytes;
    client->read_buffer[client->read_pos] = '\0';
    while (extract_line(client, line, BUFFER_SIZE))
        network_handle_client_line(server, client, line);
    return bytes;
}
