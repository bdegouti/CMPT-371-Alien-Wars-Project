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
//#include "userAction.h" --> clientAPI.h
#include "clientServerAPI.h"
#include "clientBackend.h"
#include "clientGUI.h"

struct PlayersInfo startGame() {
    connectToServer();
    struct PlayersInfo playersInfo = getPlayersInfo();
    return playersInfo;
}

// Changed by Yosup
// imp. Mutex
// move this while loop to send and recv in clientServerAPI.c
// getUserAction in sendToServer
// displayGame in getCurrentGameState (need to be imp.)
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

   // NEED TO DO:
   // terminate when game ends
   // send recv msg to displaygame() properly
   pthread_t sendThread, recvThread;
   pthread_create(&sendThread, NULL, sendToServer,(void*) game);
   pthread_create(&recvThread, NULL, getCurrentGameState, (void*) game);

   pthread_join(sendThread, NULL);
   pthread_join(recvThread, NULL);
}

int main(int argc, char *argv[]) {
    struct PlayersInfo playersInfo = startGame();

    struct Game* game = initGameState();

    print_how_to_play(playersInfo);

    playGame(playersInfo, game); 
    
    printf("test: game over since threads in client terminated\n");

    endGameState(game);

    return 0;
}