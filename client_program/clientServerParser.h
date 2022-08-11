#ifndef clientServerParser_h
#define clientServerParser_h

#include "../server_program/gameStructures.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "userAction.h"

const char *GAMESTATE;
const char *GAME_OVER;
const char *NOT_GAME_OVER;
const char *QUEUE;
const char *END_QUEUE;
const char *PLAYER;
const char *END_PLAYER;
const char *STATS;
const char *END_STATS;
const int ACTION_LENGTH;


struct Game * parseServer(char *serverMessage, struct Game *game);

#endif
