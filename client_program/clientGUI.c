#include "clientGUI.h"

const char *GUI_INTRO = "****************GAME STATS****************\n";
const char *GUI_YOUR_TEAM = "\t\tYour Team\n";
const char *GUI_ENEMY_TEAM = "\t\tEnemy Team\n";

const char *GUI_WIN = "Your team wins!!\n";
const char *GUI_LOSE = "Your team lost ._.\n";

void displayGame(struct Game *game, struct PlayersInfo info) {
    if(!game->gameover) {
        printf("%s%s", GUI_INTRO, GUI_YOUR_TEAM);
        printf("Your health\t%d\t\tAlly health\t%d\n", game->players[info.player - 1]->health, game->players[info.ally - 1]->health);
        printf("Your gun\t%d\t\tAlly gun\t%d\n", game->players[info.player - 1]->gun, game->players[info.ally - 1]->gun);
        char *playerQueue = getQueueAsString(game->players[info.player - 1]);
        printf("Your move queue\t%s", playerQueue);
        free(playerQueue);

        printf("%s", GUI_ENEMY_TEAM);
        printf("Enemy 1 health\t%d\t\tEnemy 2 health\t%d\n", game->players[info.enemy1 - 1]->health, game->players[info.enemy2 - 1]->health);
        printf("Enemy 1 gun\t%d\t\tEnemy 2 gun\t%d\n\n\n", game->players[info.enemy1 - 1]->gun, game->players[info.enemy2 - 1]->gun);
    } else {
        int winner = whoWon(game);
        if(winner == 1 && (info.player == 1 || info.ally == 1)) { //win
            printf("%s", GUI_WIN); 
        } else { //lose
            printf("%s", GUI_LOSE);
        }
    }
}


int main() {
    struct Game * game = initGameState();
    game->gameover = false;
    for(int i = 0; i < NUM_OF_PLAYERS; i++) {
        game->players[i]->gun = 70;
        game->players[i]->health = 60;
        char * test = "gun";
        char * test2 = "def";
        enqueueNewTask(game->players[i]->queue, test, i);
        enqueueNewTask(game->players[i]->queue, test2, i);
    }
    struct PlayersInfo player;
    player.ally = 2;
    player.player = 1;
    player.enemy1 = 3;
    player.enemy2 = 4;
    displayGame(game, player);
    game->players[0]->health = 0;
    game->players[1]->health = 0;
    game->gameover = true;
    displayGame(game, player);
    return 0;
}