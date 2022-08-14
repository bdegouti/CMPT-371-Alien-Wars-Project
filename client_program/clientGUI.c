#include "clientGUI.h"

const char *GUI_INTRO = "\033[32m************************\033[34mGAME STATS\033[32m**********************************\033[0m\n";
const char *GUI_YOUR_TEAM = "\t\t\t\033[32mYour Team\033[0m\n";
const char *GUI_ENEMY_TEAM = "\t\t\t\033[31mEnemy Team\033[0m\n";
const char *GUI_OUTRO = "\033[32m********************************************************************\033[0m\n";

const char *GUI_WIN = "\033[32mYour team wins!!\033[0m\n";
const char *GUI_LOSE = "\033[31mYour team lost ._.\033[0m\n";

void displayGame(struct Game *game, struct PlayersInfo info) {
    printf("%s%s", GUI_INTRO, GUI_YOUR_TEAM);
    printf("Your health:%s\t%d\t\t\t\t%sAlly health:%s\t%d%s\n", THEME_CODES[green], game->players[info.player - 1]->health, CLEAR_CODE, THEME_CODES[green], game->players[info.ally - 1]->health, CLEAR_CODE);
    printf("Your gun:\t%s%d\t\t\t\t%sAlly gun:\t%s%d%s\n", THEME_CODES[blue], game->players[info.player - 1]->gun, CLEAR_CODE, THEME_CODES[blue], game->players[info.ally - 1]->gun, CLEAR_CODE);
    
    char *playerQueue = getQueueAsString(game->players[info.player - 1]);
    char *allyQueue = getQueueAsString(game->players[info.ally - 1]);
    char *enemy1Queue = getQueueAsString(game->players[info.enemy1 - 1]);
    char *enemy2Queue = getQueueAsString(game->players[info.enemy2 - 1]);

    printf("Your queue:\033[33m\t%s%s\n", playerQueue, CLEAR_CODE);
    printf("Ally queue:\033[33m\t%s%s\n\n", allyQueue, CLEAR_CODE);
    printf("%s", GUI_ENEMY_TEAM);
    printf("Enemy 1 health:%s\t%d\t\t\t\t%sEnemy 2 health:\t%s%d%s\n", THEME_CODES[green], game->players[info.enemy1 - 1]->health, CLEAR_CODE, THEME_CODES[green], game->players[info.enemy2 - 1]->health, CLEAR_CODE);
    printf("Enemy 1 gun:%s\t%d\t\t\t\t%sEnemy 2 gun:\t%s%d%s\n", THEME_CODES[blue], game->players[info.enemy1 - 1]->gun, CLEAR_CODE, THEME_CODES[blue], game->players[info.enemy2 - 1]->gun, CLEAR_CODE);
    printf("Enemy 1 queue:\033[33m\t%s%s\n", enemy1Queue, CLEAR_CODE);
    printf("Enemy 2 queue:\033[33m\t%s%s\n", enemy2Queue, CLEAR_CODE);

    free(playerQueue);
    free(allyQueue);
    free(enemy1Queue);
    free(enemy2Queue);
    printf("%s\n", GUI_OUTRO);

    if (game->gameover)
    {
        int winner = whoWon(game);
        if (winner == 1 && (info.player == 1 || info.ally == 1))
        { // win
            printf("%s", GUI_WIN);
        }
        else
        { // lose
            printf("%s", GUI_LOSE);
        }
        return;
    }
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}


// int main() {
//     struct Game * game = initGameState();
//     game->gameover = false;
//     char *test = (char *)malloc(4);
//     char *test2 = (char *)malloc(4);
//     strcpy(test, GUN);
//     strcpy(test2, DEF);
//     test[3] = '\0';
//     test2[3] = '\0';
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
//     game->players[2]->health = 0;
//     game->players[3]->health = 0;
//     game->gameover = true;
//     displayGame(game, player);

//     // free(test);
//     // test == NULL;
//     // free(test2);
//     // test2 = NULL;
//     endGameState(game);
//     return 0;
// }

// gcc -g -o test clientGUI.c ../server_program/gameStructures.c userAction.c
// ^^^^^^^^^^ run to test