SET(LISOD_LIST liso.c io.c utilities.c client_pool.c handle_clients.c)
ADD_EXECUTABLE(client liso_client.c)
ADD_EXECUTABLE(lisod ${LISOD_LIST})
