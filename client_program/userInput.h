#ifndef userInput_h
#define userInput_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

struct PlayersInfo
{
    // id's range from 1 to 4.
    char player; // id of the client
    char ally;   // id of the client's ally
    char enemy1; // id of the clients enemy
    char enemy2; // id of the clients enemy
};

char *getUserInput(struct PlayersInfo playersInfo);

#endif