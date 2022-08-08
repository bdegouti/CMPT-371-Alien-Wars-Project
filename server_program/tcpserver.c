//gcc -pthread -o tcpserver tcpserver.c gameStructures.c to compile
#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include "gameStructures.h"


#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h> // getaddrinfo, gai_strerror

#define BACKLOG 6
#define SERVER_PORT "6969" //Server port number
#define BUFFER_SIZE 8192 //size of buffer (subject to change)
#define TURN_LENGTH 5 //length of turn (secs)

pthread_mutex_t canAccessGameData;

//for sending mulltiple args through pthread_create
struct argsToThread {
    struct Game* g;
    struct pollfd* socks;
};

void exit_if_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        exit(1);
    }

}

bool is_socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        return true;
    }
    return false;
}

struct addrinfo init_server_hints() {
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof server_hints);
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE; // pass NULL to nodename
    return server_hints;
}

struct addrinfo * init_server_addr_list() {
    struct addrinfo hints = init_server_hints();
    struct addrinfo * server_addr_list;

    int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &hints, &server_addr_list);
    exit_if_error(getaddrinfo_status, "getaddrinfo()");

    return server_addr_list;
}

/*
POLL FUNCTIONS
*/
void addSocketInPoll(struct pollfd *fds[], int newfd, int *fd_count){
    (*fds)[*fd_count].fd = newfd;
    (*fds)[*fd_count].events = POLLIN;
    (*fd_count)++;
}

void deleteSocketInPoll(struct pollfd pfds[], int i, int *fd_count){
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
}

/*
UTILITY FUNCTIONS
*/
void moveStrToHeap(char** str){
    char* tmp = *str;
    *str = malloc(sizeof(strlen(tmp)));
    strcpy(*str, tmp);
}

/*
---------------------------------------------------------------------
THE FUNCTIONS BELOW ARE RELATED TO SENDING OR RECEIVING DATA FROM CLIENTS
WE NEED CONCRETE IDEA OF WHAT MESSAGES WILL LOOK LIKE TO IMPLEMENT
---------------------------------------------------------------------
*/

//sends data to users every "round"
void* sendDataToPlayers(void* data){
    //data be an argsToThread struct*
    struct argsToThread* att = (struct argsToThread*) data;
    struct Game* g = att->g;

    //socks is a shared element, but should be threadsafe (relevant data is not changed)
    struct pollfd* socks = att->socks;
    while(true){
        sleep(TURN_LENGTH);
        pthread_mutex_lock(&canAccessGameData);
        
        //Todo: perform player actions for round

        if(g->gameover){
            //Todo: send game end data and exit 
        }

        char* gameState = ""; //Todo: getListAsString(g);
        for(int i = 0; i < NUM_OF_PLAYERS; i++){
            send(socks[i].fd, gameState, sizeof(gameState), 0);
        }
        pthread_mutex_unlock(&canAccessGameData);
    }

}

//get action via API tranlsation (DON'T REALLY NEED THIS, BUT NOT GOING TO DELETE JUST YET)
char* getActionFromAPI(char* action){
    if(strcmp(action, "att") == 0){
        return "attack";
    }
    else if(strcmp(action, "def") == 0){
        return "defence";
    }
    else if(strcmp(action, "gun") == 0){
        return "gun";
    }
    else{
        return "ukn";
    }
}

//get Target via API translation
int getTargetFromAPI(char* target){
    if(strcmp(target, "p1") == 0){
        return 0;
    }
    else if(strcmp(target, "p2") == 0){
        return 1;
    }
    else if(strcmp(target, "p3") == 0){
        return 2;
    }
    else if(strcmp(target, "p4") == 0){
        return 3;
    }
    else{
        return -1;
    }
}

//retrieve information from player transmission
void interpretPlayerMessage(struct Game* g, int player, char* msg){
    //todo
}

/*
---------------------------------------------------------------------
                END OF SEND/RECIEVE FUNCTIONS
---------------------------------------------------------------------
*/


int main() {

    pthread_mutex_init(&canAccessGameData, NULL);
    // server_addr_list 
    struct addrinfo * server_addr_list = init_server_addr_list();

    // socket 
    int socketfd = socket(PF_INET, SOCK_STREAM, 0); 
    exit_if_error(socketfd, "socket()");
    char buffer[BUFFER_SIZE];

    // bind socket and server_addr
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            continue;
        }
        break; // if we get here, we must have connected successfully
    }
    
    freeaddrinfo(server_addr_list);
    
    // listen()
    int listen_status = listen(socketfd, 20);
    exit_if_error(listen_status, "listen()");

    
    //setup poll structure (should accept 4 player connections on top of default listener)
    int currServerConnections = 0;
    struct pollfd* serverSockets = malloc(sizeof(struct pollfd) * NUM_OF_PLAYERS);


    struct sockaddr_storage clientaddr;

    //initialize Game
    int bytesSent;
    struct Game* game = initGameState();
    bool playerIsReady[4] = {false};
    bool gameStarted = false;

    //accept connections to 4 clients before proceeding into game loop
    while(currServerConnections != NUM_OF_PLAYERS) {
        socklen_t addrlen = sizeof(clientaddr);
        int newSocket = accept(socketfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(newSocket == -1){
            perror("ERROR: failed to accept connection");
        }
        else{
            addSocketInPoll(&serverSockets, newSocket, &currServerConnections);
            char startMsg[20];
            sprintf(startMsg, "Ready Player %d? (enter any key to begin)", currServerConnections);
            send(newSocket, startMsg, strlen(startMsg), 0);
        }
    }
    
    //declare thread var
    pthread_t* roundLoop;

    //use poll to scan for events on any of the connections. Enter recieved data into game.
    while(true) {
        int numberOfEvents = poll(serverSockets, NUM_OF_PLAYERS, -1);
        if(numberOfEvents == -1){
            perror("ERROR: failed at poll");
            return -1;
        }
        for(int i = 0; i < currServerConnections; i++){
            if(serverSockets[i].revents & POLLIN){
                if(gameStarted){
                    int inputBytes = recv(serverSockets[i].fd, buffer, sizeof(buffer), 0);
                    int sender_sock = serverSockets[i].fd;
                    if(inputBytes <= 0){
                        perror("ERROR: failed at recv in poll loop");
                        //todo: send exit message to clients
                        return -1;
                    }
                    pthread_mutex_lock(&canAccessGameData);
                    interpretPlayerMessage(game, i, buffer);
                    pthread_mutex_unlock(&canAccessGameData);
                }
                else{
                    playerIsReady[i] = true;
                    if(playerIsReady[0] && playerIsReady[1] && playerIsReady[2] && playerIsReady[3]){
                        gameStarted = true; 
                        //creates thread that will update gamestate and send data to players every x seconds
                        struct argsToThread att;
                        att.g = game;
                        att.socks = serverSockets;
                        if(pthread_create(roundLoop, NULL, sendDataToPlayers, &att)){
                            perror("ERROR: failed at pthread_create");
                            return -1;
                        }
                    }
                }
            }
        }
    }

    //close all sockets
    for(int i = 0; i < currServerConnections; i++){
        close(serverSockets[i].fd);
        deleteSocketInPoll(serverSockets, i, &currServerConnections);
    }
    
    //close thread
    pthread_exit(roundLoop);
    
    //free struct pollfd*
    free(serverSockets);
}
