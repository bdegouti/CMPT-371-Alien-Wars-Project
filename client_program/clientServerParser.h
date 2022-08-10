#ifndef clientServerParser_h
#define clientServerParser_h

#include "../server_program/gameStructures.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "userAction.h"

const char *GAMESTATE = "gamestate";
const char *GAME_OVER = "gameover";
const char *NOT_GAME_OVER = "notgameover";
const char *QUEUE = "queue";
const char *END_QUEUE = "endqueue";
const char *PLAYER = "player";
const char *END_PLAYER = "endplayer";
const char *STATS = "stats";
const char *END_STATS = "endstats";
const int ACTION_LENGTH = 6;


struct Game * game;

struct Game parseServer(char *serverMessage);

#endif
