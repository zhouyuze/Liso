//
// Created by zhouyuze on 26/11/2017.
//

#ifndef LISO_CLIENT_POOL_H
#define LISO_CLIENT_POOL_H

#define BUF_SIZE 8192
#define FD_SIZE (FD_SETSIZE - 1)

#include "io.h"

typedef struct {
    char* read_buf;
    int read_buf_offset;
    char* reply;
    int reply_offset;
} client;

typedef struct {
    fd_set read_fds;
    int httpfd;
    int maxfd;
    int ready;
    int count;
    int client_fd[FD_SIZE];
    client clients[FD_SIZE];
} client_pool;

extern client_pool pool;

extern void init_client_pool(int listenfd);
extern int add_client(int newfd);
extern void remove_client(int fd);
extern client * get_client(int fd);

#endif //LISO_CLIENT_POOL_H
