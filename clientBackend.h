#ifndef _clientBackend_h
#define _clientBackend_h

// create a client socket
int clientSocket (int portN);

// send and receive the game state from the server
char* clientServer (int cltSock, char * ret);

// close client side socket
void closeSocket(int cltSockt);
#endif