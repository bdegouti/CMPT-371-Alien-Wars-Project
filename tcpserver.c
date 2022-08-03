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
#define FLAGS 0
#define SERVER_PORT "6969"
#define BUFFER_SIZE 8192
#define TURN_LENGTH 5

// ------- api -------
const char client_connect_request[] = "1001001000";
const char server_connect_response[] = "0001001000";
const char client_player_action[] = "1001001001";
const char server_broadcast_gamestate[] = "0111111111";
const char server_gameover[] = "0000000000";

const char p1[] = "0001";
const char p2[] = "0010";
const char p3[] = "0011";
const char p4[] = "0100";

const char p_attack[] = "0101";
const char p_defence[] = "0110";
const char p_gun[] = "0111";
// ------- api -------

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

void addSocketInPoll(struct pollfd *fds[], int newfd, int *fd_count){
    (*fds)[*fd_count].fd = newfd;
    (*fds)[*fd_count].events = POLLIN;
    (*fd_count)++;
}

void deleteSocketInPoll(struct pollfd pfds[], int i, int *fd_count){
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
}

void moveStrToHeap(char** str){
    char* tmp = *str;
    *str = malloc(sizeof(strlen(tmp)));
    strcpy(*str, tmp);
}

//sends data to users every "round"
void* sendDataToPlayers(void* data){
    struct argsToThread* att = (struct argsToThread*) data;
    struct Game* g = att->g;
    struct pollfd* socks = att->socks;
    while(true){
        sleep(TURN_LENGTH);
        
        //Todo: perform player actions for round

        char* gameState = ""; //Todo: getListAsString(g);
        for(int i = 0; i < NUM_OF_PLAYERS; i++){
            send(socks[i].fd, gameState, sizeof(gameState), 0);
        }
    }

}

char* getActionFromAPI(char* action){
    if(strcmp(action, p_attack) == 0){
        return "att";
    }
    else if(strcmp(action, p_defence) == 0){
        return "def";
    }
    else if(strcmp(action, p_gun) == 0){
        return "gun";
    }
    else{
        return "ukn";
    }
}

int getTargetFromAPI(char* target){
    if(strcmp(target, p1) == 0){
        return 0;
    }
    else if(strcmp(target, p2) == 0){
        return 1;
    }
    else if(strcmp(target, p3) == 0){
        return 2;
    }
    else if(strcmp(target, p4) == 0){
        return 3;
    }
    else{
        return -1;
    }
}

void interpretPlayerMessage(struct Game* g, int player, char* msg){
    char* msgType = (char*) malloc(10);
    strncpy(msgType, msg, 10);


    if(strcmp(msgType, client_connect_request) == 0){
        char* targetStr = (char*) malloc(4);
        char* actionStr = (char*) malloc(4);

        //todo: check that these are valid inputs!
        strncpy(targetStr, msg+10, 4);
        strncpy(actionStr, msg+14, 4);

        int target = getTargetFromAPI(targetStr);
        char* action = getActionFromAPI(actionStr);
        
        moveStrToHeap(&action);
        
        addActionToPlayer(g, player, action, target);

        free(targetStr);
        free(actionStr);
    }
    else{
        perror("ERROR: request not understood!");
    }
    free(msgType);
}


int main() {

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

    //accept connections to 4 clients before proceeding into game loop
    while(currServerConnections != NUM_OF_PLAYERS) {
        socklen_t addrlen = sizeof(clientaddr);
        int newSocket = accept(socketfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(newSocket == -1){
            perror("ERROR: failed to accept connection");
        }
        else{
            addSocketInPoll(&serverSockets, newSocket, &currServerConnections);
            //Todo: create function to send a message to the client who just joined ("hello player x!")
        }
    }

    //creates thread that will update gamestate and send data to players every x seconds
    pthread_t* roundLoop;
    struct argsToThread att;
    att.g = game;
    att.socks = serverSockets;
    pthread_create(roundLoop, NULL, sendDataToPlayers, &att);

    //use poll to scan for events on any of the connections. Enter recieved data into game.
    while(true) {
        int numberOfEvents = poll(serverSockets, NUM_OF_PLAYERS, -1);
        if(numberOfEvents == -1){
            perror("ERROR: failed at poll");
            return -1;
        }
        for(int i = 0; i < currServerConnections; i++){
            if(serverSockets[i].revents & POLLIN){
                int inputBytes = recv(serverSockets[i].fd, buffer, sizeof(buffer), 0);
                int sender_sock = serverSockets[i].fd;
                if(inputBytes <= 0){
                    perror("ERROR: failed at recv in poll loop");
                    close(serverSockets[i].fd);
                    deleteSocketInPoll(serverSockets, i, &currServerConnections);
                }
                else if(inputBytes == 0){
                    printf("Connection to socket %d closed", serverSockets[i].fd);
                    close(serverSockets[i].fd);
                    deleteSocketInPoll(serverSockets, i, &currServerConnections);
                }

                interpretPlayerMessage(game, i, buffer);
            }
        }
    }
    free(serverSockets);
}
