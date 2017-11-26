//
// Created by zhouyuze on 26/11/2017.
//
#include "client_pool.h"

char buffer[BUF_SIZE];

void handle_request(int fd, client *c) {
    Sendn(fd, c->read_buf, c->read_buf_offset);
    memset(c->read_buf, 0, c->read_buf_offset);
    c->read_buf_offset = 0;
}

void handle_clients() {
    for (int i = 0; i < FD_SIZE && pool.ready; ++i) {
        ssize_t nbytes;
        int fd = pool.client_fd[i];

        if (fd < 0 || !FD_ISSET(fd, &pool.read_fds))
            continue;

        client *c = &pool.clients[i];
        nbytes = recv(fd, c->read_buf + c->read_buf_offset, BUF_SIZE, 0);

        printlog("[Handle Clients] Receive %d bytes from %d", nbytes, fd);

        if (nbytes == 0) {
            remove_client(fd);
        }

        c->read_buf_offset += nbytes;
        handle_request(fd, c);
        pool.ready--;
    }
}