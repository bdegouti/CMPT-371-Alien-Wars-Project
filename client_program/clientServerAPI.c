#include "clientServerAPI.h"


// startGame() functions

void connectToServer() {
    // TODO: connectToServer 
    printf("connected to server\n");
}

struct PlayersInfo getPlayersInfo() {
    // char* playersInfoMsg = recvFromServer();
    // struct PlayersInfo playersInfo = convertPlayersInfoMsg(playersInfoMsg); 

    // TEMPORARY TESTING CODE 
    struct PlayersInfo playersInfo;
    playersInfo.player = '1';
    playersInfo.ally   = '2';
    playersInfo.enemy1 = '3';
    playersInfo.enemy2 = '4';

    return playersInfo; 
}


// playGame() functions

void sendToServer(char* userAction) {
    // TODO Implement
    // sendToServer(userAction); 
    printf("User Action %s has been sent to server\n", userAction);
}

struct GameState getCurrentGameState() {
    // TODO Implement
    // char* gameStateMsg = recvFromServer(); 
    // struct GameState currentGameState = convertGameStateMsg(gameStateMsg);

    // TEMPORARY TESTING CODE
    struct GameState currentGameState;
    currentGameState.gameOver = true;
    currentGameState.gameOverMsg = "test from client server api\n";
    return currentGameState;
}

// TODO Implement
void displayGameState(struct GameState currentGameState) {
    printf("Future displayGameState\n");
}

