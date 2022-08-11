#include "clientGUI.h"

const char *GUI_INTRO = "********GAME STATE********\n";
const char *GUI_YOUR_TEAM = "\tYour Team\n";
const char *GUI_ENEMY_TEAM = "\tEnemy Team\n";

const char *GUI_WIN = "Your team wins!!\n";
const char *GUI_LOSE = "Your team lost ._.\n";

void displayGame(struct Game *game, struct PlayersInfo info) {
    if(!game->gameover) {
        printf("%s%s", GUI_INTRO, GUI_YOUR_TEAM);
        printf("Your health %d\tAlly health %d\n", game->players[info.player - 1]->health, game->players[info.ally - 1]->health);
        printf("Your gun %d\tAlly gun %d\n", game->players[info.player - 1]->gun, game->players[info.ally - 1]->gun);
        printf("%s", GUI_ENEMY_TEAM);
        printf("Enemy 1 health %d\Enemy 2 health %d\n", game->players[info.enemy1 - 1]->health, game->players[info.enemy2 - 1]->health);
        printf("Enemy gun %d\Enemy gun %d\n\n\n", game->players[info.enemy1 - 1]->gun, game->players[info.enemy2 - 1]->gun);
    } else {
        int winner = whoWon(game);
        if(winner == 1 && (info.player == 1 || info.ally == 1)) { //win
            printf("%s", GUI_WIN); 
        } else { //lose
            printf("%s", GUI_LOSE);
        }
    }
}
