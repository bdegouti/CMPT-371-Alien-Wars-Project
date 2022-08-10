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

//randomizes which player goes first.
void randomizePlayOrder(int** players){
    size_t i;
    for (i = 0; i < NUM_OF_PLAYERS-1; i++){
        size_t j = i + rand() / (RAND_MAX / (NUM_OF_PLAYERS - i) + 1);
        int t = *players[j];
        *players[j] = *players[i];
        *players[i] = t;
    }
}

//execeutes a  single round of the game
void executeRound(struct Game* g){
    int playOrder[] = {0, 1, 2, 3};
    randomizePlayOrder(&playOrder);
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        struct Action* pAction = getCurrentActionForPlayer(g, playOrder[i]);
        applyTask(g, pAction->target, pAction->action);
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
                struct Action *act = dequeueCurrentTask(g->players[playerNum]->queue);
                return act;
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
    if (strcmp(a->action, "att") == 0){   // ANDY CLIENTTOSERVERAPI please change attack to att
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
        if (g->players[a->target]->health <= 0){
            g->players[a->target]->health = 0;
            if (whoWon(g) != 0){
                g->gameover = true;
            }
        }
    }
    else if (strcmp(a->action, "def") == 0){ // ANDY CLIENTTOSERVERAPI please change defense to def 
        p->health += 10;
    }
    else if (strcmp(a->action, "gun") == 0){  // ANDY CLIENTTOSERVERAPI please change boost to gun 
        p->gun++;
        if (p->gun == MAX_GUN_FILL){
            p->isBoostActive = true;
            p->boostCount = BOOST_COUNT_START;
        }
    }
}

int whoWon(struct Game *g){
    if (g->players[2]->health == 0 && g->players[3]->health == 0) {
        return 1;  // Team 1 won.
    } else if (g->players[0]->health == 0 && g->players[1]->health == 0) {
        return 2;  // Team 2 won.
    } else return 0; // Game not over.
}

// Example: player 1 queue att 2 att 2 att 1 att 2 att 1 endqueue stats 13 16 endstats endplayer
char* getQueueNodeAsString(struct Player *p){
    char* str = malloc(BUFFER_SIZE);

    char playerNum[1];
    char playerHealth[1];
    char playerGun[1];

    sprintf(playerNum, "%d", p->num);
    sprintf(playerHealth, "%d", p->health);
    sprintf(playerGun, "%d", p->gun);

    strcpy (str, "gamestate gamenotover player "); 
    strcpy (str, playerNum); 
    strcpy (str, " queue ");

    struct Action *temp = (struct Action*)malloc(sizeof(struct Action));
    temp = createNode(p->queue->head->action, p->queue->head->target);
    char tempTargetNum[1];
    
    for (int i = 0; i < p->queue->size; i++){
        
        sprintf(tempTargetNum, "%d", temp->target);
        
        strcpy (str, temp->action);
        strcpy (str, " ");
        strcpy (str, tempTargetNum);
        strcpy (str, " ");

        temp = temp->next;
    }
    free(temp);
    temp = NULL;

    strcpy (str, " endqueue stats ");
    strcpy (str, playerHealth);
    strcpy (str, " ");
    strcpy (str, playerGun);
    strcpy (str, " endstats endplayer ");
}

char *getGameStateAsString(struct Game *g){
    char* str = malloc(BUFFER_SIZE);

    if (g->gameover == true) 
        return "gamestate gameover endgamestate";
    else{
        strcpy (str, "gamestate gamenotover"); 

        for (int i = 0; i < NUM_OF_PLAYERS; i++){
            char* tmp = getQueueNodeAsString(g->players[i]);
            strcpy (str, tmp);
            free(tmp);
        } 
        strcpy (str, " endgamestate");
    }

    return str;
}
