#ifndef userAction_h
#define userAction_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "clientGameDataStructures.h"

const char *ATT = "att";
const char *DEF = "def";
const char *GUN = "gun";

char* getUserAction(struct PlayersInfo playersInfo);


#endif