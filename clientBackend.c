#include "clientBackend.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 100

// send and receive the game state from the server
char* clientServer (int cltSock, char * ret) {
    char buffer[BUF_SIZE];
    char* gameState = (char*)malloc(BUF_SIZE);

    write(cltSock, ret, strlen(ret));
    read(cltSock, buffer, BUF_SIZE);

    strcpy (gameState, buffer);

   return gameState;
}
