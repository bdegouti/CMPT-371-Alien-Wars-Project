#include "clientGUI.h"

const char *GUI_INTRO = "************************GAME STATS************************\n";
const char *GUI_YOUR_TEAM = "\t\t\tYour Team\n";
const char *GUI_ENEMY_TEAM = "\t\t\tEnemy Team\n";

const char *GUI_WIN = "Your team wins!!\n";
const char *GUI_LOSE = "Your team lost ._.\n";

void displayGame(struct Game *game, struct PlayersInfo info) {
    if(!game->gameover) {
        printf("%s%s", GUI_INTRO, GUI_YOUR_TEAM);
        printf("Your health\t%d\t\t\t\tAlly health\t%d\n", game->players[info.player - 1]->health, game->players[info.ally - 1]->health);
        printf("Your gun\t%d\t\t\t\tAlly gun\t%d\n", game->players[info.player - 1]->gun, game->players[info.ally - 1]->gun);
        
        char *playerQueue = getQueueAsString(game->players[info.player - 1]);
        printf("Your queue:\t%s\n", playerQueue);
        free(playerQueue);

        char *allyQueue = getQueueAsString(game->players[info.ally - 1]);
        printf("Ally queue:\t%s\n\n", allyQueue);
        free(allyQueue);

        printf("%s", GUI_ENEMY_TEAM);
        printf("Enemy 1 health\t%d\t\t\t\tEnemy 2 health\t%d\n", game->players[info.enemy1 - 1]->health, game->players[info.enemy2 - 1]->health);
        printf("Enemy 1 gun\t%d\t\t\t\tEnemy 2 gun\t%d\n", game->players[info.enemy1 - 1]->gun, game->players[info.enemy2 - 1]->gun);

        char *enemy1Queue = getQueueAsString(game->players[info.enemy1 - 1]);
        printf("Enemy 1 queue:\t%s\n", enemy1Queue);
        free(enemy1Queue);

        char *enemy2Queue = getQueueAsString(game->players[info.enemy2 - 1]);
        printf("Enemy 2 queue:\t%s\n\n\n", enemy2Queue);
        free(enemy2Queue);
    } else {
        int winner = whoWon(game);
        if(winner == 1 && (info.player == 1 || info.ally == 1)) { //win
            printf("%s", GUI_WIN); 
        } else { //lose
            printf("%s", GUI_LOSE);
        }
    }
}


// int main() {
//     struct Game * game = initGameState();
//     game->gameover = false;
//     char *test = (char *)malloc(4);
//     char *test2 = (char *)malloc(4);
//     strcpy(test, GUN);
//     strcpy(test2, DEF);
//     for(int i = 0; i < NUM_OF_PLAYERS; i++) {
//         game->players[i]->gun = 70;
//         game->players[i]->health = 60;
//         enqueueNewTask(game->players[i]->queue, test, i);
//         enqueueNewTask(game->players[i]->queue, test2, i);
//     }
//     struct PlayersInfo player;
//     player.ally = 2;
//     player.player = 1;
//     player.enemy1 = 3;
//     player.enemy2 = 4;
//     displayGame(game, player);
//     game->players[0]->health = 0;
//     game->players[1]->health = 0;
//     game->gameover = true;
//     displayGame(game, player);
//     free(test);
//     free(test2);
//     return 0;
// }

// gcc -o test clientGUI.c ../server_program/gameStructures.c
// ^^^^^^^^^^ run to test