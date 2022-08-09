#ifndef clientBackend_h
#define clientBackend_h


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "clientGameDataStructures.h"

#define SERVER_PORT "6969"
#define MAXBUFFERBYTES 100

// return boolean when error
bool socket_error(int status, char* error_message);

// exit when error
void exit_socket_error(int status, char* error_message);

// set to IPv4, TCP, flags=AI_PASSIVE, respectively
struct addrinfo init_server_hints();

// create a client socket and connect to the server
void connectServer();

struct PlayersInfo serverGivePlayersInfo ();

// send command to the server
void sendAction (char* ret);

// receive the game state
char* recvGameState();

// close socket
void closeSocket();

#endif