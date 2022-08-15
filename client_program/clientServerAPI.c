#include "clientServerAPI.h"

struct PlayersInfo playersInfoInClientBack;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

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

    printf("received player info = %s\n", playersInfoMsg);
    return playersInfoInClientBack; 
}

void* sendToServer(void* game) {
    struct Game* gameTemp = (struct Game*)game;

    while (gameTemp->gameover == false){
        char* userAction = getUserAction(playersInfoInClientBack);
        
        // mutex send may not be necessary???
        pthread_mutex_lock(&mutex);
        sendAction(userAction);
        pthread_mutex_unlock(&mutex);
        //sleep(1);

        free(userAction);

        if (gameTemp->gameover){
            printf("game over in send\n");
        }        
    }

    return NULL;
}

void* getCurrentGameState(void* game) {
    struct Game* gameTemp = (struct Game*)game;

    while (gameTemp->gameover == false){
        char* gameStateMsg = recvState();
        //printf("in client = %s\n", gameStateMsg);

        pthread_mutex_lock(&mutex);
        gameTemp = parseServer(gameStateMsg, gameTemp);
        displayGame(gameTemp, playersInfoInClientBack);
        pthread_mutex_unlock(&mutex);

        if (gameTemp->gameover){
            printf("test: gameover in recv\n");
            // free(gameStateMsg);
        }
        //sleep(1);
    }

   return NULL;
}
