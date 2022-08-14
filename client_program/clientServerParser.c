#include "clientServerParser.h"

const char *GAMESTATE = "gamestate";
const char *GAME_OVER = "gameover";
const char *NOT_GAME_OVER = "notgameover";
const char *QUEUE = "queue";
const char *END_QUEUE = "endqueue";
const char *PLAYER = "player";
const char *END_PLAYER = "endplayer";
const char *STATS = "stats";
const char *END_STATS = "endstats";
const int ACTION_LENGTH = 6;

// All functions in this file are helper functions for parseServer() 

bool checkGameState(char *message) {
    return !strncmp(message, GAMESTATE, strlen(GAMESTATE));
}

//return true if game over
bool getGameState(char *message) {
    return !strncmp(message, GAME_OVER, strlen(GAME_OVER));
}

int getPlayerNum(char *message) {
    message += strlen(PLAYER) + 1;
    return message[0] - '0'; //convert char digit to int
}

void getNextAction(char *message, struct PlayerQueue *queue) {
    char * move = (char *)malloc(strlen(ATT) + 1); 
    strncpy(move, message, strlen(ATT));
    move[strlen(ATT)] = '\0';
    message += strlen(ATT) + 1;

    int target = message[0];
    message += 2;

    enqueueNewTask(queue, move, target);
    free(move);
    move = NULL;
}

int getStats(char *message) {
    printf("message 0 = %c\n", message[0]);
    printf("message 1 = %c\n", message[1]);
    printf("message 2 = %c\n", message[2]);
    if(message[1] == ' ') { //stat only has 2 digits
        long digitOne = strtol(message, &message, 10);
        message++;
        long digitTwo = strtol(message, &message, 10);
        message += 2;
        //return (int) (digitOne * 10) + digitTwo; //this returns 900 instead 90??
        return (int) (digitOne) + digitTwo; 
    }
    if(message[2] == ' ') { //stat only has 2 digits
        long digitOne = strtol(message, &message, 10);
        message++;
        long digitTwo = strtol(message, &message, 10);
        message += 2;
        //return (int) (digitOne * 10) + digitTwo; //this returns 900 instead 90??
        return (int) (digitOne) + digitTwo; 
    }
    else {
        long digitZero = 100;
        message++;
        long digitOne = strtol(message, &message, 10);
        message++;
        long digitTwo = strtol(message, &message, 10);
        message += 2;
        return (int)(digitOne * 10) + digitTwo + digitZero;
    }
}

struct Player *getPlayer(char * message, struct Game * game) {
    int playerNum = getPlayerNum(message);
    struct Player * player = game->players[playerNum];
    message += 2; //slide pointer to start of queue
    message += strlen(QUEUE) + 1;
    while(strncmp(message, END_QUEUE, strlen(END_QUEUE))) {
        getNextAction(message, player->queue);
    }        
    message += strlen(END_QUEUE) + 1;
    message += strlen(STATS) + 1;

    player->health = getStats(message);
    player->gun = getStats(message);

    message += strlen(END_STATS) + 1;
    message += strlen(END_PLAYER) + 1;
    return player;
}

void updateGame (char * message, struct Game * game) {
    int playerNum;
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        playerNum = i;
        printf("chopped 1 = %s\n\n",message);
        message += strlen(PLAYER) + 1;
        message += 2; //slide pointer to start of queue
        message += strlen(QUEUE) + 1;
        printf("chopped 2 = %s\n\n",message);
        while(strncmp(message, END_QUEUE, strlen(END_QUEUE))) {
            //getNextAction(message, game->players[playerNum]->queue);
            message += strlen(ATT) + 1;
            message += 2;
        } 
        message += strlen(END_QUEUE) + 1;
        message += strlen(STATS) + 1;

        printf("chopped 3 = %s\n\n",message);

        game->players[playerNum]->health = getStats(message);
        game->players[playerNum]->gun = getStats(message);

        if (game->players[playerNum]->health == 100) {
            message += 6;    
        }
        else if (game->players[playerNum]->health < 100 && game->players[playerNum]->health > 0) {
            message += 5;
        }
        else {
            message += 4;
        }
        message += strlen(END_STATS) + 1;
        message += strlen(END_PLAYER) + 1;

        printf("chopped end = %s\n\n",message);
    }
}

struct Game * parseServer(char *serverMessage, struct Game * game) {
    if(!checkGameState(serverMessage)){
        //gameover
        game->gameover = true;
        return game;
    }

    char *message = serverMessage; //copy to a working copy
    message += strlen(GAMESTATE) + 1;
    
    bool gameOver = checkGameState(message);
    game->gameover = gameOver;
    message += gameOver ? strlen(GAME_OVER) + 1 : strlen(NOT_GAME_OVER) + 1;
    //printf("msg in paser = %s\n", message);
    updateGame(message, game);
    /*
    for(int i = 0; i < NUM_OF_PLAYERS; i++) {
        getPlayer(message, game);
    }
    */
    free(serverMessage);
    serverMessage = NULL;
    return game;
}
