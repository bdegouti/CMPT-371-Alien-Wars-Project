#include "clientBackend.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdio.h>

#define BUF_SIZE 100
#define Port_Num 9090

//connect client to server
int clientSocket (char* ipAdr) {
    int cltSock;
    struct sockaddr_in cltAdr;

    if((cltSock = socket(PF_INET, SOCK_STREAM, 0)) == -1){ printf("error sock");}

    memset(&cltAdr, 0, sizeof(cltAdr));
    cltAdr.sin_family = AF_INET;
    //cltAdr.sin_addr.s_addr = htonl(INADDR_ANY); //localhost ipAdr automatically
    cltAdr.sin_addr.s_addr = inet_addr(ipAdr);
    cltAdr.sin_port = htons(Port_Num);

    if(connect(cltSock, (struct sockaddr*)&cltAdr, sizeof(cltAdr)) == -1) {printf("error connect");}
    return cltSock; //returns the socket
}


// send and receive the game state from the server
char* clientServer (int cltSock, char * ret) {
    char buffer[BUF_SIZE];
    char* gameState = (char*)malloc(BUF_SIZE);

    write(cltSock, ret, strlen(ret));
    read(cltSock, buffer, BUF_SIZE);

    strcpy (gameState, buffer);

   return gameState;
}

//close socket
void closeSocket (int cltSock) {
    close(cltSock);
}