#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <pthread.h>

#include "clientGameDataStructures.h" // IMPORTANT: Please View!
#include "../server_program/gameStructures.h"
#include "playerGameInstructions.h"
#include "clientServerAPI.h"
#include "clientBackend.h"
#include "clientGUI.h"

struct PlayersInfo startGame() {
    connectToServer();
    struct PlayersInfo playersInfo = getPlayersInfo();
    return playersInfo;
}

void playGame(struct PlayersInfo playersInfo, struct Game* game) {
//void playGame(struct PlayersInfo playersInfo) {
    /* before mutex
    while (!game->gameover) {
        char* userAction = getUserAction(playersInfo);
        sendToServer(userAction);
        getCurrentGameState(game);
        displayGame(game, playersInfo);
    }
    */
   pthread_t sendThread, recvThread;

   pthread_create(&sendThread, NULL, sendToServer, (void*) game);
   pthread_create(&recvThread, NULL, getCurrentGameState, (void*) game);

   pthread_join(sendThread, NULL);
   pthread_join(recvThread, NULL);
}

int main(int argc, char *argv[]) {
    struct PlayersInfo playersInfo = startGame();

    struct Game* game = initGameState();

    print_how_to_play(playersInfo);

    playGame(playersInfo, game); 
    
    closeSocket();
    printf("test: game over since threads in client terminated\n");

    endGameState(game);

    return 0;
}