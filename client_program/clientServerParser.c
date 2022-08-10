#include "clientServerParser.h"

bool initUpdate = true;

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
    char move; 
    strncmpy(message, move, strlen(ATT));
    message += strlen(ATT) + 1;

    int target = message[0];
    message += 2;

    enqueueNewTask(queue, move, target);
}

int getStats(char *message) {
    if(message[2] = ' ') { //stat only has 2 digits
        long digitOne = strtol(message, &message, 10);
        message++;
        long digitTwo = strtol(message, &message, 10);
        message += 2;
        return (int) (digitOne * 10) + digitTwo;
    } else {
        long digitZero = 100;
        message++;
        long digitOne = strtol(message, &message, 10);
        message++;
        long digitTwo = strtol(message, &message, 10);
        message += 2;
        return (int)(digitOne * 10) + digitTwo + digitZero;
    }
}

struct Player *getPlayer(char * message, struct Player * player) {
    int playerNum = getPlayerNum(message);
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
}

struct Game parseServer(char *serverMessage) {
    if(!checkGameState(serverMessage)) return;

    if(initUpdate) {
        game = initGameState();
        initUpdate = false;
    }

    char *message = serverMessage; //copy to a working copy
    message += strlen(GAMESTATE) + 1;
    
    bool gameOver = checkGameState(message);
    message += gameOver ? strlen(GAME_OVER) + 1 : strlen(NOT_GAME_OVER) + 1;
    
    for(int i = 0; i < NUM_OF_PLAYERS; i++) {
        getPlayer(message, game->players[i]);
    }
    
}