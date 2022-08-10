#ifndef CLIENT_GUI_H
#define CLIENT_GUI_H

#include "clientServerParser.h"
#include "../server_program/gameStructures.h"
#include "clientGameDataStructures.h"

const char * GUI_INTRO = "********GAME STATE********\n";
const char * GUI_YOUR_TEAM = "\tYour Team\n";
const char * GUI_ENEMY_TEAM = "\tEnemy Team\n";

const char * GUI_WIN = "Your team wins!!\n";
const char *GUI_LOSE = "Your team lost ._.\n";

void displayGame(struct Game *game, struct PlayersInfo *info);

#endif