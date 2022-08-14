#ifndef clientServerAPI_h
#define clientServerAPI_h

#include <stdio.h>
#include <unistd.h> //for sleep
#include <pthread.h>

#include "clientGameDataStructures.h"
#include "../server_program/gameStructures.h"
#include "clientServerParser.h"
#include "clientBackend.h"
#include "userAction.h"
#include "clientGUI.h"

void connectToServer();
struct PlayersInfo getPlayersInfo();

void* sendToServer(void* game);
void* getCurrentGameState(void* game);

#endif