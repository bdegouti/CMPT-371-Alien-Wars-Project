#ifndef CLIENT_GUI_H
#define CLIENT_GUI_H

#include "clientServerParser.h"
#include "../server_program/gameStructures.h"
#include "clientGameDataStructures.h"

const char * GUI_INTRO;
const char * GUI_YOUR_TEAM;
const char * GUI_ENEMY_TEAM;

const char * GUI_WIN;
const char *GUI_LOSE;

void displayGame(struct Game *game, struct PlayersInfo info);
    
#endif