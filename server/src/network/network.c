/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** network
*/

#include "server.h"

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
