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

#include<arpa/inet.h>

#include "clientGameDataStructures.h"
//#define SERVER_ADDR "207.23.165.176" // school
//#define SERVER_ADDR "24.207.13.89" //to other machine
//#define SERVER_ADDR "192.168.250.1/24"
#define SERVER_ADDR "192.168.1.101"
#define SERVER_PORT "8080"
#define MAXBUFFERBYTES 300

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