// this is for client-clientbackend example to test client works
// this is for client-clientbackend example to test client works
// this is for client-clientbackend example to test client works

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include "userInput.h"

#include "clientBackend.h"

bool gameOver = false; // true when game over

// move to clientBackend
// TODO: Client Server 
/*
void server_give_players_info(struct PlayersInfo* playersInfo) {
    playersInfo->player = '1';
    playersInfo->ally =   '2';
    playersInfo->enemy1 = '3';
    playersInfo->enemy2 = '4';
}
*/

int main(int argc, char *argv[]) {
    // On running of the client program, the client will try to connect to the server 
    connectServer(); // Here, the client should have connected to the server. 
    printf("You have connected to the server. Please wait for other players to join \n");

    // it tests only a single connection, not multiple
    // The client waits for all other players to connect to the server. 
    //printf("All players have been connected to the server. \n");
    
    // The client recieves some information about gamestate from the server
    //printf("You are player 1. Your teammate is player 2. Your enemies are players 3 and 4.\n");
    // lets assume the server will return these values somehow. 
    //server_give_players_info(&playersInfo);
    
    struct PlayersInfo playersInfo = serverGivePlayersInfo();
    printf("player = %c\n", playersInfo.player);
    printf("ally = %c\n", playersInfo.ally);
    printf("enemy1 = %c\n", playersInfo.enemy1);
    printf("enemy2 = %c\n", playersInfo.enemy2);
    
    //struct PlayersInfo playersInfo;

    // If the client recieves game data, will it be printed while the user is being asked for input?
    while (!gameOver) {
        char *valid_move = getUserInput(playersInfo); //Remember to free !!

        // Send to the server
        sendAction(valid_move);
        char *gameState = recvState();
        printf("In client = %s\n", gameState);
                
        free(gameState);        
        free(valid_move);
        //valid_move = NULL; // Why? 
    }
    

}
