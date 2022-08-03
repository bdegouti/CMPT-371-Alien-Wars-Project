#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include "userInput.h"

bool gameOver = false; // true when game over

// TODO: Client Server
bool send_to_server(char * valid_move) {
    printf("%s\n", valid_move);
    return true; //temporary return
}

// TODO: Client Server 
void server_give_players_info(struct PlayersInfo* playersInfo) {
    playersInfo->player = '1';
    playersInfo->ally =   '2';
    playersInfo->enemy1 = '3';
    playersInfo->enemy2 = '4';
}

void print_how_to_play(struct PlayersInfo playersInfo) {
    printf("\n");
    printf("The goal of the game is to kill your enemies, without dying yourselves.\n");
    // Teaching client how to write a move. 
    printf("Attack the enemy with `att %c` or `att %c`.\n", playersInfo.enemy1, playersInfo.enemy2);
    printf("Defend your team with `def %c` or `def %c`.\n", playersInfo.player, playersInfo.ally);
    printf("Load your special gun with `gun`.\n");
    // Gun Mechanic
    printf("Once your special gun is fully loaded, other players cannot load their gun, and your attacks do 2x damage for 5 turns.\n");
    printf("\n");
}


int main(int argc, char *argv[]) {
    // On running of the client program, the client will try to connect to the server 
    // Here, the client should have connected to the server. 
    printf("You have connected to the server. Please wait for other players to join \n");

    // The client waits for all other players to connect to the server. 
    printf("All players have been connected to the server. \n");
    
    // The client recieves some information about gamestate from the server
    printf("You are player 1. Your teammate is player 2. Your enemies are players 3 and 4.\n");
    // lets assume the server will return these values somehow. 
    struct PlayersInfo playersInfo;
    server_give_players_info(&playersInfo);

    print_how_to_play(playersInfo);

    // If the client recieves game data, will it be printed while the user is being asked for input?
    while (!gameOver) {
        char *valid_move = getUserInput(playersInfo); //Remember to free !!

        // Send to the server
        if (send_to_server(valid_move)) {
            printf("Your move was sent to the server\n");
        } else {
            printf("Your move wasn't sent to the server, please try again\n");
        }

        free(valid_move);
        valid_move = NULL; // Why? 
    }
}