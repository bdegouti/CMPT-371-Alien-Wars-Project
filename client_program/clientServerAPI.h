#ifndef clientServerAPI_h
#define clientServerAPI_h

#include <stdio.h>

#include "clientGameDataStructures.h"
#include "../server_program/gameStructures.h"
#include "clientServerParser.h"
#include "clientBackend.h" //added by Yosup

// startGame() functions

void connectToServer();

struct PlayersInfo getPlayersInfo();

// playGame() functions

void sendToServer(char* userAction); 

struct Game * getCurrentGameState(struct Game * game);

#endif