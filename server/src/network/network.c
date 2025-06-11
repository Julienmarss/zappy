/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network implementation
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

static command_t *create_command(const char *line)
{
    command_t *cmd = calloc(1, sizeof(command_t));

    if (!cmd)
        return NULL;
    cmd->cmd = strdup(line);
    if (!cmd->cmd) {
        free(cmd);
        return NULL;
    }
    cmd->time_limit = 0;
    cmd->next = NULL;
    return cmd;
}

static void add_command_to_queue(client_t *client, command_t *cmd)
{
    command_t *last = client->cmd_queue;

    if (!client->cmd_queue) {
        client->cmd_queue = cmd;
    } else {
        while (last->next)
            last = last->next;
        last->next = cmd;
    }
    client->cmd_count++;
}

static void handle_client_line(server_t *server, client_t *client,
    const char *line)
{
    command_t *cmd = NULL;

    if (client->type == CLIENT_UNKNOWN) {
        network_handle_new_client(server, client, line);
    } else if (client->type == CLIENT_PLAYER) {
        if (client->cmd_count < MAX_CMD_QUEUE) {
            cmd = create_command(line);
            if (cmd)
                add_command_to_queue(client, cmd);
        }
    }
}

int network_receive(server_t *server, client_t *client)
{
    int bytes = 0;
    char line[BUFFER_SIZE];

    bytes = read(client->fd, client->read_buffer + client->read_pos,
        BUFFER_SIZE - client->read_pos - 1);
    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 1;  // Not an error, just no data available
        return -1;
    }
    if (bytes == 0)
        return 0;  // Client closed connection
    client->read_pos += bytes;
    client->read_buffer[client->read_pos] = '\0';
    while (extract_line(client, line, BUFFER_SIZE))
        handle_client_line(server, client, line);
    return bytes;
}
