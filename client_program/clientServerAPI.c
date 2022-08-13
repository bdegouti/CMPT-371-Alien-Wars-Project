#include "clientServerAPI.h"

struct PlayersInfo playersInfoInClientBack;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

// startGame() functions
void connectToServer() {
    connectServer();
    printf("connected to server\n");
}

// send in the order of player, ally, enemy 1 and 2
// for example, 2, 1, 3, 4; if the player is assigned 2
struct PlayersInfo getPlayersInfo() {
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

void* sendToServer(void* game) {
    struct Game* gameTemp = (struct Game*)game;
    while (1){
        if (gameTemp->gameover){
            printf("game over in send\n");
            return NULL;
        }
        char* userAction = getUserAction(playersInfoInClientBack);
        // mutex send may not be necessary
        //pthread_mutex_lock(&mutex);
        sendAction(userAction);
        //pthread_mutex_unlock(&mutex);
        //sleep(1);
    }

    return NULL;
}

void* getCurrentGameState(void* game) {

    struct Game* gameTemp = (struct Game*)game;

    while (1){
        //char* gameStateMsg = recvState();
        // testing purpose
        /*
        pthread_mutex_lock(&mutex);
        printf("in client = %s\n", recvState());
        pthread_mutex_unlock(&mutex);
        */
        
        pthread_mutex_lock(&mutex);
        game = parseServer(recvState(), game);
        displayGame(gameTemp, playersInfoInClientBack);
        if (gameTemp->gameover){printf("test: gameover in recv\n"); return NULL;}
        pthread_mutex_unlock(&mutex);

        //sleep(1);
    }


   return NULL;

}