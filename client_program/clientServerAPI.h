#ifndef clientServerAPI_h
#define clientServerAPI_h

#include <stdio.h>

#include "clientGameDataStructures.h"
#include "../server_program/gameStructures.h"
#include "clientServerParser.h"
#include "clientBackend.h"

// THESE HEADERS CAME FROM client.c
#include "userAction.h"
#include "clientGUI.h"

//for thread
#include <unistd.h> //for sleep
#include <pthread.h>

// startGame() functions

void connectToServer();

struct PlayersInfo getPlayersInfo();

// playGame() functions

// changed for mutex form
//   keep the original for future
//      void sendToServer(char* userAction); 
//      struct Game * getCurrentGameState(struct Game * game);
void* sendToServer(void* game);
void* getCurrentGameState(void* game);

#endif