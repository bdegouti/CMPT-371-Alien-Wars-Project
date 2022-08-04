//clientBackend.c userInput.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include "userInput.h"

#include "clientBackend.h"


bool gameOver = false; // true when game over

// TODO: Client Server 
void server_give_players_info(struct PlayersInfo* playersInfo) {
    playersInfo->player = '1';
    playersInfo->ally =   '2';
    playersInfo->enemy1 = '3';
    playersInfo->enemy2 = '4';
}


int main(int argc, char *argv[]) {


    // On running of the client program, the client will try to connect to the server 
    connectServer(); // Here, the client should have connected to the server. 
    printf("You have connected to the server. Please wait for other players to join \n");

    // The client waits for all other players to connect to the server. 
    //printf("All players have been connected to the server. \n");
    
    //char* receivePlayerInfo = recvGameState();
    //printf("recv info = %s\n",receivePlayerInfo);
    
    // The client recieves some information about gamestate from the server
    //printf("You are player 1. Your teammate is player 2. Your enemies are players 3 and 4.\n");
    // lets assume the server will return these values somehow. 
    struct PlayersInfo playersInfo;
    playersInfo.player = '1';
    //server_give_players_info(&playersInfo);

    // If the client recieves game data, will it be printed while the user is being asked for input?
    

    while (!gameOver) {
        char *valid_move = getUserInput(playersInfo); //Remember to free !!

        // Send to the server
        sendAction(valid_move);
        //char *gameState = recvGameState();
        //printf("from server = %s\n", gameState);
        
        char *ret = recvGameState();
        printf("from server = %s\n", ret);


        free(valid_move);
        free(ret);
        //valid_move = NULL; // Why? 
    }
    

}
