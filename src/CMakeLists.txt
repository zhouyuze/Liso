SET(LISOD_LIST liso.c io.c server.c utilities.c)
ADD_EXECUTABLE(client liso_client.c)
ADD_EXECUTABLE(lisod ${LISOD_LIST})
