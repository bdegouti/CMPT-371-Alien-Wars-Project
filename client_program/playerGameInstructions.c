#include "playerGameInstructions.h"

void print_how_to_play(struct PlayersInfo playersInfo) {
    printf("\n");
    printf("The goal of the game is to kill your enemies, without dying yourselves.\n");
    // Teaching client how to write a move. 
    printf("Attack the enemy with `att %c` or `att %c`.\n", playersInfo.enemy1, playersInfo.enemy2);
    printf("Defend your team with `def %c` or `def %c`.\n", playersInfo.player, playersInfo.ally);
    printf("Load your special gun with `gun`.\n");
    // Gun Mechanic
    printf("Once your special gun is fully loaded, other players cannot load their gun, and your attacks do 2x damage for 5 turns.\n");
    printf("\n");
}