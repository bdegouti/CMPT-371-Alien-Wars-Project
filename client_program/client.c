// make, ./client 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#include "clientGameDataStructures.h" // IMPORTANT: Please View! 
#include "playerGameInstructions.h"
#include "userAction.h"
#include "clientServerAPI.h"
#include "clientBackend.h"


struct PlayersInfo startGame() {
    connectToServer();
    struct PlayersInfo playersInfo = getPlayersInfo();
    return playersInfo;
}

// Returns endgame gameState
struct GameState playGame(struct PlayersInfo playersInfo) {
    struct GameState currentGameState;
    currentGameState.gameOver = false;
    currentGameState.gameOverMsg = "You Won!"; // ONLY FOR TESTING

    while (!currentGameState.gameOver) {
        char* userAction = getUserAction(playersInfo);
        sendToServer(userAction);
        currentGameState = getCurrentGameState();
        displayGameState(currentGameState);
    }

    return currentGameState;
}

// TODO: Needs Work - What does GameState Look Like? 
void gameOver(struct GameState endGameState) {
    printf("%s \n", endGameState.gameOverMsg);
    
    printf("Game Over!\n");
}


int main(int argc, char *argv[]) {
    struct PlayersInfo playersInfo = startGame();

    print_how_to_play(playersInfo);

    struct GameState endGameState = playGame(playersInfo); 

    gameOver(endGameState);
}