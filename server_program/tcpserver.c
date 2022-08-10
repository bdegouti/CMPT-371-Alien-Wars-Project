//gcc -pthread -o tcpserver tcpserver.c gameStructures.c to compile
#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include "gameStructures.c"
#include "client_to_server_api.c"


#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h> // getaddrinfo, gai_strerror

#define BACKLOG 6
#define SERVER_PORT "6954" //Server port number
#define TURN_LENGTH 5 //length of turn (secs)

#define ALL_PLAYER_ARRIVED_MESSAGE  "AllPlayersHere"
#define GAME_START_MESSAGE "GameStarted"

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

void sendToEachPlayer(char* msg, struct pollfd* serverSockets){
    int bytes;
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        bytes = send(serverSockets[i].fd, msg, strlen(msg), 0);
        if(bytes < 0){
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, BUFFER_SIZE, "Error:failed to send round data to p%d",(i+1));
            perror(buffer);
        }
    }
}

//sends data to users every "round"
void* roundDataSender(void* data){
    //data be an argsToThread struct*
    struct argsToThread* att = (struct argsToThread*) data;
    struct Game* game = att->g;

    //socks is a shared element, but should be threadsafe (relevant data is not changed)
    struct pollfd* socks = att->socks;
    while(true){
        //pauses for length of turn
        sleep(TURN_LENGTH);
        pthread_mutex_lock(&canAccessGameData);
        
        //executes round of game
        executeRound(game);

        //gets gameState as string
        char* gameState = getGameStateAsString(game);
        sendToEachPlayer(gameState, socks);
        free(gameState);

        if(game->gameover){
            pthread_mutex_unlock(&canAccessGameData);
            break;
        }

        pthread_mutex_unlock(&canAccessGameData);
    }

}

//sends gamesetup info to players (player number, teammates player number, enemy numbers)
char* getIntroduction(int i){
    char* introMsg = (char*) malloc(SMALL_BUFFER);
    if((i+1) == 1){
        strcpy(introMsg, "1234\0");
    }
    else if((i+1) == 2){
        strcpy(introMsg, "2134\0");
    }
    else if((i+1) == 3){
        strcpy(introMsg, "3412\0");
    }
    else if((i+1) == 4){
        strcpy(introMsg, "4312\0");
    }
    else{
        perror("Error: invalid player number at getIntroduction");
        exit(-1);
    }
}

//retrieve information from player transmission
void interpretPlayerMessage(struct Game* g, int player, char* msg){
    struct PlayerAction PlayerAction = getPlayerAction(player, msg);
    addActionToPlayer(g, player, PlayerAction.action, PlayerAction.targetPlayer);
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
    int option = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    exit_if_error(socketfd, "socket()");
    char buffer[BUFFER_SIZE];

    // bind socket and server_addr
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            exit(-1);
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
    struct Game* game = initGameState();
    bool playerIsReady[4] = {false};
    bool gameStarted = false;
    bool GameNotEnded = true;

    //accept connections to 4 clients before proceeding into game loop
    while(currServerConnections != NUM_OF_PLAYERS) {
        socklen_t addrlen = sizeof(clientaddr);
        int newSocket = accept(socketfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(newSocket == -1){
            perror("ERROR: failed to accept connection");
        }
        else{
            addSocketInPoll(&serverSockets, newSocket, &currServerConnections);

            //message to send to player upon connecting; may not use

            /*
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, BUFFER_SIZE, "p%d\n", currServerConnections);
            send(newSocket, buffer, strlen(buffer), 0);
            */
        }
    }
    
    if(close(socketfd) < 0){
        perror("failed to close socket");
    } 

    //tell players that all players have arrived
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        char* introMsg = getIntroduction(i);
        send(serverSockets[i].fd, introMsg, strlen(SMALL_BUFFER), 0);
    }

    //declare thread var
    pthread_t roundLoop;

    //use poll to scan for events on any of the connections. Enter recieved data into game.
    while(GameNotEnded) {
        int numberOfEvents = poll(serverSockets, NUM_OF_PLAYERS, 50);
        if(numberOfEvents == -1){
            perror("ERROR: failed at poll");
            return -1;
        }
        for(int i = 0; i < currServerConnections; i++){
            /*
            this if/else prevents the game from starting the round count until every player
            has confirmed that they are ready
            */
            if(serverSockets[i].revents & POLLIN){
                //buffer cleared
                memset(buffer, 0, BUFFER_SIZE);

                //here is where the server retrieves the message associated with the poll event
                int inputBytes = recv(serverSockets[i].fd, buffer, sizeof(buffer), 0);

                if(gameStarted){

                    /*FOR TESTING PURPOSES*/
                    printf("from client %d: %s\n", i+1, buffer);

                    if(inputBytes <= 0){
                        if(inputBytes == 0){
                            printf("player %d has left", i+1);
                        }
                        else{
                            perror("ERROR: failed at recv in poll loop");
                            //todo: send exit message to clients
                        }
                        GameNotEnded = false;
                        break;
                    }
                    //using the mutex to ensure that the game struct is accessed in a thread safe manner
                    pthread_mutex_lock(&canAccessGameData);
                    interpretPlayerMessage(game, i, buffer);
                    pthread_mutex_unlock(&canAccessGameData);
                }
                else{
                    playerIsReady[i] = true;

                    /*FOR TESTING PURPOSES*/
                    printf("p1: %d / p2: %d / p3: %d / p4: %d\n", playerIsReady[0], playerIsReady[1], playerIsReady[2], playerIsReady[3]);

                    if(playerIsReady[0] && playerIsReady[1] && playerIsReady[2] && playerIsReady[3]){
                        gameStarted = true;
                        snprintf(buffer, BUFFER_SIZE, GAME_START_MESSAGE);
                        sendToEachPlayer(buffer, serverSockets);
                        //creates thread that will update gamestate and send data to players every x seconds
                        
                        //this is the struct that passes arguments to gameLoop
                        struct argsToThread att;
                        att.g = game;
                        att.socks = serverSockets;
                        
                        //gameLoop thread is launched--the game has started
                        if(pthread_create(&roundLoop, NULL, roundDataSender, &att)){
                            perror("ERROR: failed at pthread_create");
                            GameNotEnded = false;
                            break;
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

    endGameState(game);
    
    //free struct pollfd*
    free(serverSockets);
}
