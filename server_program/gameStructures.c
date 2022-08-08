#include "gameStructures.h"

// creates PlayerQueue
struct PlayerQueue *createPlayerQueue(){
    struct PlayerQueue *pq = (struct PlayerQueue *)malloc(sizeof(struct PlayerQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
}

// creates taskNode (used in enqueueNewTask, do not use elsewhere)
struct Action *createNode(char *action, int target){
    struct Action *act = (struct Action*)malloc(sizeof(struct Action));
    act->action = action;
    act->target = target;
    act->next = NULL;
    return act;
}

// adds new task to end of player queue
void enqueueNewTask(struct PlayerQueue *pq, char *action, int target){
    if (pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return;
    }

    struct Action *act = createNode(action, target);
    if (pq->head == NULL && pq->tail == NULL){
        pq->head = act;
        pq->tail = pq->head;
    }
    else{
        pq->tail->next = act;
        pq->tail = act;
    }
    pq->size++;
}

// removes task from front of player queue
struct Action *dequeueCurrentTask(struct PlayerQueue *pq){
    if (pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return NULL;
    }

    struct Action *currAction = pq->head;

    if (pq->head != pq->tail){
        pq->head = pq->head->next;
        currAction->next = NULL;
        return currAction;
    }
    else{
        pq->head = NULL;
        pq->tail = pq->head;
        return currAction;
    }
}

void deletePlayerQueue(struct PlayerQueue *pq){
    struct Action *tmp = NULL, *act = pq->head;
    while (act != NULL){
        tmp = act;
        free(act->action);
        act = act->next;
        free(tmp);
        tmp = NULL;
    }
    free(pq);
    pq = NULL;
}

struct Player *createPlayer(int num){
    struct Player *p = (struct Player *)malloc(sizeof(struct Player));
    p->num = num;
    p->gun = DEFAULT_GUN;
    p->health = DEFAULT_HEALTH;
    p->queue = createPlayerQueue();
    p->isBoostActive = false;
    p->boostCount = 0;
}

void deletePlayer(struct Player *p){
    deletePlayerQueue(p->queue);
    free(p);
    p = NULL;
}

struct Game *initGameState(){
    struct Game *gameState = (struct Game *)malloc(sizeof(struct Game));
    gameState->gameover = false;
    gameState->gunlocked = false;
    for (int i = 0; i < NUM_OF_PLAYERS; i++){
        gameState->players[i] = createPlayer(i + 1);
    }
    return gameState;
}

void endGamestate(struct Game *g){
    for (int i = 0; i < NUM_OF_PLAYERS; i++){
        deletePlayer(g->players[i]);
    }
    free(g);
    g = NULL;
}

void addActionToPlayer(struct Game *g, int playerNum, char *action, int target){
    if (playerNum < NUM_OF_PLAYERS)
    {
        enqueueNewTask(g->players[playerNum]->queue, action, target);
    }
}

// If a player's next command is "gun boost" while another player is using the gun boost. That command is being ignored.
struct Action *getCurrentActionForPlayer(struct Game *g, int playerNum){
    if (playerNum < NUM_OF_PLAYERS){
        for (int i = 0; i < NUM_OF_PLAYERS; i++){
            if (g->players[i]->isBoostActive == true){
                printf("Gun boost is unavailable!\nAnother player is using it.\nAdding the next command!");
                dequeueCurrentTask(g->players[playerNum]->queue);
                getCurrentActionForPlayer(g, playerNum);
            }
            else{
                struct Action *a = (struct Action *)malloc(sizeof(struct Action));
                struct Action *act = dequeueCurrentTask(g->players[playerNum]->queue);
                free(act->action);
                free(act);
                a->action = act->action;
                a->target = act->target;
                return a;
            }
        }
    }
    return NULL;
}

/* Applies the task and updates the queue of the player
   Attacks do 10 damage
   Boosted attacks do 20 damage
*/

void applyTask(struct Game *g, struct Player *p, struct Action *a){
    if (a->action == "attack"){   // ANDY CLIENTTOSERVERAPI please change attack to att
        if (p->isBoostActive){
            g->players[a->target]->health -= 20;
            p->boostCount--;
            if (p->boostCount == 0){
                p->isBoostActive = false;
            }
        }
        else{
            g->players[a->target]->health -= 10;
        }
        if (g->players[a->target]->health == 0){
            deletePlayer(g->players[a->target]);
            if (whoWon(g) != 0){
                g->gameover = true;
            }
        }
    }
    else if (a->action == "defense"){ // ANDY CLIENTTOSERVERAPI please change defense to def 
        p->health += 10;
    }
    else if (a->action == "boost"){  // ANDY CLIENTTOSERVERAPI please change boost to gun 
        p->gun++;
        if (p->gun == MAX_GUN_FILL){
            p->isBoostActive = true;
            p->boostCount = BOOST_COUNT_START;
        }
    }
}

int whoWon(struct Game *g){
    if (g->players[2] == NULL && g->players[3] == NULL) {
        return 1;  // Team 1 won.
    } else if (g->players[0] == NULL && g->players[1] == NULL) {
        return 2;  // Team 2 won.
    } else return 0;
}


char *getGameStateAsString(struct Game *g){
    return NULL;
}
