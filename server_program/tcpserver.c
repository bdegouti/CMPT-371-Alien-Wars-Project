//gcc -pthread -o tcpserver tcpserver.c gameStructures.c to compile
#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h> // getaddrinfo, gai_strerror
#include <signal.h>

#include "gameStructures.h"
#include "client_to_server_api.c"

//my laptops ip address is 24.207.13.89

#define BACKLOG 6
#define SERVER_PORT "8080" //Server port number
#define SERVER_PORT_AS_INT 8080 // not used in currrent setup
#define SERVER_ADDR "24.207.13.89" //not used in current setup
#define TURN_LENGTH 5 //length of turn (secs)
#define GAME__DOESNT_DELAY_START true

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
    server_hints.ai_socktype = SOCK_STREAM; // TCP
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

/*
 *retrieve information from player transmission
 */
void interpretPlayerMessage(struct Game* g, int player, char* msg){
    struct PlayerAction PlayerAction = getPlayerAction(player, msg);
    addActionToPlayer(g, player, PlayerAction.action, PlayerAction.targetPlayer);
}

//sends data to users every "round"
void* roundDataSender(void* data){
    //data be an argsToThread struct*
    struct argsToThread* att = (struct argsToThread*) data;
    struct Game* game = att->g;

    //socks is a shared element, but should be threadsafe (relevant data is not changed)
    struct pollfd* socks = att->socks;
    int roundNum = 0;
    while(true){
        //pauses for length of turn
        sleep(TURN_LENGTH);
        pthread_mutex_lock(&canAccessGameData);
        
        //executes round of game
        executeRound(game);
        printf("round %d\n", roundNum);
        roundNum++;

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
    return NULL;
}

/*
 *sends gamesetup info to players (player number, teammates player number, enemy numbers)
 */
char* getIntroduction(int i){
    char* introMsg = (char*) malloc(BUFFER_SIZE);
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
    return introMsg;
}

/*
 * this function enters into a loop, waiting until all players have connected and been accepted by the server to procede
 */
void waitForAllPlayersToJoin(int socketfd, struct pollfd** serverSockets){
    int currServerConnections = 0;
    struct sockaddr_storage clientaddr;
    while(currServerConnections != NUM_OF_PLAYERS) {
        printf("waiting on clients\n");
        socklen_t addrlen = sizeof(clientaddr);
        int newSocket = accept(socketfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(newSocket == -1){
            perror("ERROR: failed to accept connection");
        }
        else{
            addSocketInPoll(serverSockets, newSocket, &currServerConnections);
            
            /*FOR TESTING PURPOSES*/
            printf("connected to client\n");
        }
    }
}


/*
 * This function runs the game, recieving messages from the clients and sending that data into
 * the gameStructure, allowing roundDataSender to determine the server responses. the struct pollfd
 * allows for the server to watch all four player sockets for
 */
void runGame(struct pollfd** serverSockets){
    //declare thread var
    pthread_t roundLoop;

    int currServerConnections = NUM_OF_PLAYERS;

    //initialize Game
    struct Game* game = initGameState();
    bool playerIsReady[4] = {false};
    bool gameStarted = GAME__DOESNT_DELAY_START;
    bool GameNotEnded = true;
    char buffer[BUFFER_SIZE];

    //if the game is not delaying for all players to confirm readiness, start immediately
    if(GAME__DOESNT_DELAY_START){
        struct argsToThread att;
        att.g = game;
        att.socks = *serverSockets;

        if(pthread_create(&roundLoop, NULL, roundDataSender, &att)){
            perror("ERROR: failed at pthread_create");
            exit(-1);
        }
    }

    //use poll to scan for events on any of the connections. Enter recieved data into game.
    while(GameNotEnded) {
        int numberOfEvents = poll(*serverSockets, NUM_OF_PLAYERS, -1);
        if(numberOfEvents == -1){
            perror("ERROR: failed at poll");
            return;
        }
        for(int i = 0; i < NUM_OF_PLAYERS; i++){
            /*
            this if/else prevents the game from starting the round count until every player
            has confirmed that they are ready unless GAME_DOESNT_DELAY_START
            */
            if((*serverSockets)[i].revents & POLLIN){
                //buffer cleared
                memset(buffer, 0, BUFFER_SIZE);

                //here is where the server retrieves the message associated with the poll event
                int inputBytes = recv((*serverSockets)[i].fd, buffer, sizeof(buffer), 0);
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
                else if(gameStarted){

                    /*FOR TESTING PURPOSES*/
                    printf("from client %d: %s\n", i+1, buffer);

                    //using the mutex to ensure that the game struct is accessed in a thread safe manner
                    pthread_mutex_lock(&canAccessGameData);
                    interpretPlayerMessage(game, i+1, buffer);
                    pthread_mutex_unlock(&canAccessGameData);
                }
                else{
                    if(!playerIsReady[i]){
                        snprintf(buffer, BUFFER_SIZE, "Player %d is ready!", (i+1));
                        sendToEachPlayer(buffer, *serverSockets);
                    }

                    playerIsReady[i] = true;

                    /*FOR TESTING PURPOSES*/
                    printf("p1: %d / p2: %d / p3: %d / p4: %d\n", playerIsReady[0], playerIsReady[1], playerIsReady[2], playerIsReady[3]);

                    if(playerIsReady[0] && playerIsReady[1] && playerIsReady[2] && playerIsReady[3]){
                        
                        /*FOR TESTING PURPOSES*/
                        printf("game is now starting!\n");

                        gameStarted = true;
                        snprintf(buffer, BUFFER_SIZE, GAME_START_MESSAGE);
                        sendToEachPlayer(buffer, *serverSockets);
                        //creates thread that will update gamestate and send data to players every x seconds
                        
                        //this is the struct that passes arguments to gameLoop
                        struct argsToThread att;
                        att.g = game;
                        att.socks = *serverSockets;
                        
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
        close((*serverSockets)[i].fd);
        deleteSocketInPoll(*serverSockets, i, &currServerConnections);
    }
    
    //close thread
    pthread_exit(&roundLoop);

    endGameState(game);
    
    //free struct pollfd*
    free(*serverSockets);
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
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    exit_if_error(socketfd, "socket()");

    // bind socket and server_addr
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            exit(-1);
        }
        break; // if we get here, we must have connected successfully
    }
    
    freeaddrinfo(server_addr_list);

    //setup poll structure (should accept 4 player connections on top of default listener)
    struct pollfd* serverSockets = malloc(sizeof(struct pollfd) * NUM_OF_PLAYERS);
    
    // listen()
    int listen_status = listen(socketfd, 4);
    exit_if_error(listen_status, "listen()");

    //accept connections to 4 clients before proceeding into game loop
    waitForAllPlayersToJoin(socketfd, &serverSockets);
    
    if(close(socketfd) < 0){
        perror("failed to close socket");
    } 


    //tell players that all players have arrived, startup data (teams and enemies)
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        char* introMsg = getIntroduction(i);
        send(serverSockets[i].fd, introMsg, strlen(introMsg), 0);
        free(introMsg);
    }

    //runs the game until it reaches it's end (or a player quits)
    runGame(&serverSockets);
    printf("end");
}
