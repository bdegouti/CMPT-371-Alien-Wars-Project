#include "gameStructures.h"

// creates PlayerQueue
struct PlayerQueue *createPlayerQueue(){
    struct PlayerQueue *pq = (struct PlayerQueue *)malloc(sizeof(struct PlayerQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    return pq;
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
    if(pq->size == MAX_SIZE_OF_QUEUE){
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
        pq->size--;
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
    return p;
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

void endGameState(struct Game *g){
    for (int i = 0; i < NUM_OF_PLAYERS; i++){
        deletePlayer(g->players[i]);
    }
    free(g);
    g = NULL;
}

void addActionToPlayer(struct Game *g, int playerNum, char *action, int target){
    if (playerNum < NUM_OF_PLAYERS)
    {
        enqueueNewTask(g->players[playerNum-1]->queue, action, target);
    }
}

//randomizes which player goes first.
int* randomizePlayOrder(){
    int* playOrder = (int*) malloc(NUM_OF_PLAYERS*sizeof(int));
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        playOrder[i] = i;
    }

    size_t i;
    for (i = 0; i < NUM_OF_PLAYERS-1; i++){
        size_t j = i + rand() / (RAND_MAX / (NUM_OF_PLAYERS - i) + 1);
        int t = playOrder[j];
        playOrder[j] = playOrder[i];
        playOrder[i] = t;
    }
    return playOrder;
}

//execeutes a  single round of the game
int* executeRound(struct Game* g){
    int* gunCheck = calloc(sizeof(int), NUM_OF_PLAYERS);
    int* playOrder = randomizePlayOrder();
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        struct Action* pAction = getCurrentActionForPlayer(g, playOrder[i]);
        if(pAction != NULL){
            if(strcmp(pAction->action, "gun") == 0 && g->gunlocked){
                gunCheck[playOrder[i]] = 1;
            }
            else{
                applyTask(g, g->players[playOrder[i]], pAction);
            }
        }
    }
    free(playOrder);
    return gunCheck;
}

// If a player's next command is "gun boost" while another player is using the gun boost. That command is being ignored.
struct Action *getCurrentActionForPlayer(struct Game *g, int playerNum){
    struct Action *act = dequeueCurrentTask(g->players[playerNum]->queue);
    return act;
}

/* Applies the task and updates the queue of the player
   Attacks do 10 damage
   Boosted attacks do 20 damage
*/

void applyTask(struct Game *g, struct Player *p, struct Action *a){
    if(p->health > 0){
        if (strcmp(a->action, "att") == 0){   // ANDY CLIENTTOSERVERAPI please change attack to att
            if (p->isBoostActive){
                g->players[(a->target)-1]->health -= 20;
                p->boostCount--;
                if (p->boostCount == 0){
                    p->isBoostActive = false;
                    g->gunlocked = false;
                }
            }
            else{
                g->players[(a->target)-1]->health -= 10;
            }
            if (g->players[(a->target)-1]->health <= 0){
                g->players[(a->target)-1]->health = 0;
                if (whoWon(g) != 0){
                    g->gameover = true;
                }
            }
        }
        else if (strcmp(a->action, "def") == 0){ // ANDY CLIENTTOSERVERAPI please change defense to def 
            if(g->players[(a->target)-1]->health < 100){
                g->players[(a->target)-1]->health += 10;
            }
        }
        else if (strcmp(a->action, "gun") == 0){  // ANDY CLIENTTOSERVERAPI please change boost to gun 
            if(p->gun < MAX_GUN_FILL){
                p->gun++;
            }
            if (p->gun == MAX_GUN_FILL){
                p->isBoostActive = true;
                p->boostCount = BOOST_COUNT_START;
                g->gunlocked = true;
            }
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

char* getQueueAsString(struct Player *p) {
    char *str = (char *)malloc(p->queue->size * 8);
    struct Action *temp = p->queue->head;
    char tempTargetNum[SMALL_BUFFER];

    strcpy(str, "");
    while (temp != NULL)
    {
        memset(tempTargetNum, 0, SMALL_BUFFER);
        snprintf(tempTargetNum, SMALL_BUFFER, "%d", temp->target);

        strcat(str, temp->action);
        strcat(str, " ");
        strcat(str, tempTargetNum);
        strcat(str, ", ");
        temp = temp->next;
    }
    str[strlen(str) - 2] = '\0';
    return str;
}

// Example: player 1 queue att 2 att 2 att 1 att 2 att 1 endqueue stats 13 16 endstats endplayer
char* getQueueNodeAsString(struct Player *p){
    char* str = (char*) malloc(BUFFER_SIZE);

    char playerNum[SMALL_BUFFER];
    char playerHealth[SMALL_BUFFER];
    char playerGun[SMALL_BUFFER];

    snprintf(playerNum, SMALL_BUFFER, "%d", (p->num));
    snprintf(playerHealth, SMALL_BUFFER, "%d", p->health);
    snprintf(playerGun, SMALL_BUFFER, "%d", p->gun);

    strcpy (str, "player "); 
    
    strcat (str, playerNum); 
    strcat (str, " queue ");

    struct Action *temp = p->queue->head;
    char tempTargetNum[SMALL_BUFFER];
    
    while(temp != NULL){
        memset(tempTargetNum, 0, SMALL_BUFFER);
        snprintf(tempTargetNum, SMALL_BUFFER, "%d", temp->target);
        
        strcat (str, temp->action);
        strcat (str, " ");
        strcat (str, tempTargetNum);
        strcat (str, " ");

        temp = temp->next;
    }

    strcat (str, "endqueue stats ");
    strcat (str, playerHealth);
    strcat (str, " ");
    strcat (str, playerGun);
    strcat (str, " endstats endplayer ");
    
    return str;
}

char *getGameStateAsString(struct Game *g){
    char* str = (char*) malloc(BUFFER_SIZE);
    strcpy (str, "gamestate ");
    if(g->gameover){
        strcat(str, "gameover ");
    }
    else{
        strcat(str, "gamenotover ");
    }

    for (int i = 0; i < NUM_OF_PLAYERS; i++){
        char* tmp = getQueueNodeAsString(g->players[i]);
        strcat (str, tmp);
        free(tmp);
    } 
    strcat (str, "endgamestate\0");

    return str;
}

