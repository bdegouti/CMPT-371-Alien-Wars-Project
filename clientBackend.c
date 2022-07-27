#include "clientBackend.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdio.h>

#define BUF_SIZE 100
#define DATA_PAYLOAD 255

////////////////////
////API Example/////
////////////////////
enum ttype {
    conn = 10,
    act = 20,
    over = 30
};

typedef struct Message {
	int direct; // 0 for S->C 1 for C->S
    enum ttype type; //types of data in enum
    char message[DATA_PAYLOAD]; //data that contains action or gamestate
}message; 
////////////////////
////API Example/////
////////////////////

//connect client to server
int clientSocket (int portN) {
    int cltSock;
    struct sockaddr_in cltAdr;

    if((cltSock = socket(PF_INET, SOCK_STREAM, 0)) == -1){ printf("error sock");}

    memset(&cltAdr, 0, sizeof(cltAdr));
    cltAdr.sin_family = AF_INET;
    // this connects to the localhost
    // if user specify the ip then change is needed
    cltAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    cltAdr.sin_port = htons((portN));

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