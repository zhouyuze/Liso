#include "liso.h"
#include "client_pool.h"
#include "handle_clients.h"

int main(int args, char **argv) {

  /* Check usage */
  if (args != 2) {
    printlog(USAGE, argv[0]);
    exit(1);
  }

  /* Paramters from terminal */
  int port = atoi(argv[1]);

  /* Temp parameters */
  int listenfd, newfd;
  sockaddr_in clientaddr;
  socklen_t addrlen = sizeof(sockaddr_in);
  char remoteIP[INET6_ADDRSTRLEN];

  /* Create server's only listener socket */
  printlog("[Main] ************Liso Echo Server*********");
  if ((listenfd = open_listenfd(port)) < 0) {
    printlog("[Main] Can not open port %d", port);
    exit(1);
  }
  printlog("[Main] Successfully create listener socket %d", listenfd);

  /* Init client pool */
  init_client_pool(listenfd);

  while(1) {
    printlog("[Main] Selecting...");
    pool.ready = select(pool.maxfd+1, &pool.read_fds, NULL, NULL, NULL);

    if (FD_ISSET(listenfd, &pool.read_fds)) {
      /* Handle new incoming connections from client */
      newfd = accept(listenfd, (sockaddr *)&clientaddr, &addrlen);

      printlog("[Main] New connection from %s on socket %d",
               inet_ntop(clientaddr.sin_family,
                         get_in_addr((sockaddr *) &clientaddr),
                         remoteIP,
                         INET6_ADDRSTRLEN),
               newfd);

      add_client(newfd);
      pool.ready--;
    }

    if (pool.ready > 0) {
      handle_clients();
    }
  }
}
