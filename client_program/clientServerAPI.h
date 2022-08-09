#ifndef clientServerAPI_h
#define clientServerAPI_h

#include <stdio.h>
#include "clientGameDataStructures.h"

#include "clientBackend.h" //added by Yosup


// startGame() functions

void connectToServer();

struct PlayersInfo getPlayersInfo();


// playGame() functions

void sendToServer(char* userAction); 

struct GameState getCurrentGameState();

void displayGameState(struct GameState currentGameState);


#endif