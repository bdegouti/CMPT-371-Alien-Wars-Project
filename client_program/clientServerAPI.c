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
    
    printf("received player info = %s\n", playersInfoMsg);
    return playersInfoInClientBack; 
}

// playGame() functions

//void sendToServer(char* userAction) {
void* sendToServer(void* game) {
    // TODO Implement
    // sendToServer(userAction); 

    //below is newly implmented with thread
    //NOT IMP.: termination when the game over
    struct Game* gameTemp = (struct Game*)game;
    while (1){
        //pthread_mutex_lock(&mutex);
        if (gameTemp->gameover){
            printf("game over in send\n");
            return NULL;
        }
        char* userAction = getUserAction(playersInfoInClientBack);
        sendAction(userAction);
        //pthread_mutex_unlock(&mutex);
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

    struct Game* gameTemp = (struct Game*)game;
    while (1){
    //while (1){
        //pthread_mutex_lock(&mutex);
        char* gameStateMsg = recvState();

        // testing purpose
        //printf("in client = %s\n", gameStateMsg);
        //if (!strcmp(gameStateMsg, "gun 2")){ printf("compare\n"); gameTemp->gameover = true;}
        //if (gameTemp->gameover){printf("game over\n"); return NULL;}
        
        game = parseServer(gameStateMsg, game);
        displayGame(gameTemp, playersInfoInClientBack);
        if (gameTemp->gameover){printf("test: gameover in recv\n"); return NULL;}
        
        //pthread_mutex_unlock(&mutex);
    }


   return NULL;

}