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

// for "socklen_t" struct
//#include<arpa/inet.h>
//#define SERVER_PORT_AS_INT 8080

#include "clientGameDataStructures.h"

#define SERVER_PORT "8080" //Server port number
//#define SERVER_ADDR "24.207.13.89" // server address
#define SERVER_ADDR "127.0.0.1" // test for localhost
#define BUFFER_SIZE 8192 // to be changed if server change the size

// return boolean when error
bool socket_error(int status, char* error_message);

// exit when error
void exit_socket_error(int status, char* error_message);

// set to IPv4, TCP, flags=AI_PASSIVE, respectively
struct addrinfo init_server_hints();

// create a client socket and connect to the server
void connectServer();

// send command to the server
void sendAction (char* ret);

// receive the game state
char* recvState();

// close socket
void closeSocket();

#endif