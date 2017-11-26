//
// Created by zhouyuze on 26/11/2017.
//

#include "client_pool.h"

client_pool pool;

void init_client_pool(int listenfd) {
    FD_ZERO(&pool.read_fds);
    FD_SET(listenfd, &pool.read_fds);
    pool.httpfd = listenfd;
    pool.maxfd = listenfd;
    pool.ready = 0;
    pool.count = 0;

    client *c = NULL;
    for (int i = 0; i < FD_SIZE; ++i) {
        pool.client_fd[i] = -1;
        c = &pool.clients[i];

        c->read_buf = (char *) malloc(sizeof(char) * BUF_SIZE);
        memset(c->read_buf, 0, BUF_SIZE);
        c->read_buf_offset = 0;

        c->reply = (char *) malloc(sizeof(char) * BUF_SIZE);
        memset(c->reply, 0, BUF_SIZE);
        c->reply_offset = 0;
    }
}

int add_client(int newfd) {
    if (pool.count == FD_SIZE) {
        return -1;
    }

    pool.maxfd = pool.maxfd > newfd ? pool.maxfd : newfd;
    FD_SET(newfd, &pool.read_fds);
    for (int i = 0; i < FD_SIZE; ++i) {
        if (pool.client_fd[i] == -1) {
            pool.client_fd[i] = newfd;
            pool.count++;
            printlog("[Client Pool] Add new client %d at %d.", newfd, i);
            return i;
        }
    }

    // TODO error
    return -1;
}

void remove_client(int fd) {
    FD_CLR(fd, &pool.read_fds);

    if (pool.maxfd == fd) {
        pool.maxfd = pool.httpfd;
    }

    for (int i = 0; i < FD_SIZE; ++i) {
        if (pool.client_fd[i] == fd) {
            pool.count--;
            pool.client_fd[i] = -1;

            client *c = &pool.clients[i];
            c->read_buf_offset = 0;
            c->reply_offset = 0;
            memset(c->read_buf, 0, BUF_SIZE);
            memset(c->reply, 0, BUF_SIZE);
            printlog("[Client Pool] Remove client %d at %d.", fd, i);
        } else
            pool.maxfd = pool.maxfd > pool.client_fd[i] ? pool.maxfd : pool.client_fd[i];
    }
}

client * get_client(int fd) {
    for (int i = 0; i < FD_SIZE; ++i) {
        if (pool.client_fd[i] == fd)
            return &pool.clients[i];
    }
    return NULL;
}
