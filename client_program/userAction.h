#ifndef userAction_h
#define userAction_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "clientGameDataStructures.h"

const char * ATT;
const char * DEF;
const char * GUN;
const char *THEME_CODES[5];
const char *CLEAR_CODE;

char* getUserAction(struct PlayersInfo playersInfo);


#endif