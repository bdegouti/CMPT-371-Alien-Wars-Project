#include "clientServerAPI.h"

struct PlayersInfo playersInfoInClientBack;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

// startGame() functions

void connectToServer() {
    // TODO: connectToServer 
    connectServer();
    printf("connected to server\n");
}

struct PlayersInfo getPlayersInfo() {
    // assume after connect, server sends one msg
    // by knowing who to send
    // send in the order of player, ally, enemy 1 and 2
    // for example, 2, 1, 3, 4; if the player is assigned 2

    char* playersInfoMsg = recvState();
    playersInfoInClientBack.player = playersInfoMsg[0];
    playersInfoInClientBack.ally = playersInfoMsg[1];
    playersInfoInClientBack.enemy1 = playersInfoMsg[2];
    playersInfoInClientBack.enemy2 = playersInfoMsg[3];
    // struct PlayersInfo playersInfo = convertPlayersInfoMsg(playersInfoMsg); 
    //UNCOMMENT ABOVE LINE AFTER TESTING ^^^^^^^^^^^^^^^^^^^^^^^^^^
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    // TEMPORARY TESTING CODE 
    /*
    struct PlayersInfo playersInfo;
    playersInfo.player = '1';
    playersInfo.ally   = '2';
    playersInfo.enemy1 = '3';
    playersInfo.enemy2 = '4';
    */
    //printf("test p1 = %c\n", playersInfoInClientBack.player);
    
    return playersInfoInClientBack; 
}

// playGame() functions

//void sendToServer(char* userAction) {
void* sendToServer(void* ptr) {
    // TODO Implement
    // sendToServer(userAction); 

    //below is newly implmented with thread
    //NOT IMP.: termination when the game over
    while (1){
        pthread_mutex_lock(&mutex);
        char* userAction = getUserAction(playersInfoInClientBack);    
        sendAction(userAction);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* getCurrentGameState(void* game) {

    // TODO Implement
    // char* gameStateMsg = recvFromServer(); 
    
    /* before mutex imp.
    char* gameStateMsg = recvState();
    // struct GameState currentGameState = convertGameStateMsg(gameStateMsg);
    game = parseServer(gameStateMsg, game);
    */

    while (1){
        pthread_mutex_lock(&mutex);
        char* gameStateMsg = recvState();
        game = parseServer(gameStateMsg, game);
        //printf("\nin client = %s\n", gameStateMsg);
        //displayGame((struct Game*) game, playersInfoInClientBack);
        pthread_mutex_unlock(&mutex);
    }


   return NULL;
}
