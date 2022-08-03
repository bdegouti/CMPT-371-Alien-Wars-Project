#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

//enum for console colors
enum theme {
    white,
    red,
    blue,
    green
};

//theme codes for console colors
const char *THEME_CODES[5] = {"\033[37m",
                              "\033[31m",
                              "\033[34m",
                              "\033[32m"};
const char *CLEAR_CODE = "\033[0m"; //clears all console formatting

const char INPUT_ERROR[] = "Please enter a valid move!\nMoves are att, def, gun\n\n";

const char* ATT = "att";
const char* DEF = "def";
const char* GUN = "gun";

bool gameOver = false; // true when game over


struct PlayersInfo {
    // id's range from 1 to 4.
    char player; // id of the client
    char ally;  // id of the client's ally
    char enemy1; // id of the clients enemy
    char enemy2; // id of the clients enemy
};

// gets user input
// checks for user input error
// returns valid user move 
char* getUserInput(struct PlayersInfo playersInfo) {
    char buffer[7]; // input size of 5 + \n + \0
    char* ret = (char *) malloc(6); // 5 + \0

    // loop till input valid 
    bool input_not_valid = true;
    while (input_not_valid) {
        // get user input
        printf("%s<move> <player target> %sEnter your move: %s", THEME_CODES[blue], THEME_CODES[green], THEME_CODES[red]);
        fgets(buffer, 7, stdin);
        buffer[5] = '\0'; //remove the \n from string 
        printf("%s", CLEAR_CODE); // clear text formatting

        // check user input valid 
        if (strncmp(buffer, ATT, 3) == 0) { // Attack
            if (buffer[4] == playersInfo.enemy1 || buffer[4] == playersInfo.enemy2) {
                strcpy(ret, buffer);
                input_not_valid = false;
            } else {
                printf("Invalid Player Target!\n");
            }
        } else if (strncmp(buffer, DEF, 3) == 0) { // Defense
            if (buffer[4] == playersInfo.player || buffer[4] == playersInfo.ally) {
                strcpy(ret, buffer);
                input_not_valid = false;
            } else {
                printf("Invalid Player Target!\n");
            }
        } else if (strncmp(buffer, GUN, 3) == 0) { // Loading Gun
            strcpy(ret, GUN);
            input_not_valid = false;
        } 
        
        if (input_not_valid) {
            printf("%s is not a valid move.\n", buffer);
            printf("%s%s%s", THEME_CODES[red], INPUT_ERROR, CLEAR_CODE); //print error and moves
        }
    }

    return ret; 
}

// TODO: Client Server
bool send_to_server(char * valid_move) {
    printf("%s\n", valid_move);
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