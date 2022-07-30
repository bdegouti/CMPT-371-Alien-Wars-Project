#include "gameStructures.h"

//creates PlayerQueue
struct PlayerQueue* createPlayerQueue(){
    struct PlayerQueue *pq = (struct PlayerQueue*) malloc(sizeof(struct PlayerQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
}

//creates taskNode (used in enqueueNewTask, do not use elsewhere)
struct ListNode* createNode(char* action, int target) {
    struct ListNode *ls = (struct ListNode*) malloc(sizeof(struct ListNode));
    ls->action = action;
    ls->target = target;
    ls->next = NULL;
    return ls;
}

//adds new task to end of player queue
void enqueueNewTask (struct PlayerQueue* pq, char* action, int target){
    if(pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return;
    }
    
    struct ListNode* ln = CreateNode(action, target);
    if(pq->head == NULL && pq->tail == NULL){
        pq->head = ln;
        pq->tail = pq->head;
    }
    else{
        pq->tail->next = ln;
        pq->tail = ln;
    }
    pq->size++;
}

//removes task from front of player queue
struct ListNode* dequeueCurrentTask(struct PlayerQueue* pq){
    if(pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return;
    }

    struct ListNode* currLN = pq->head;

    if(pq->head != pq->tail){
        pq->head = pq->head->next;
        currLN->next = NULL;
        return currLN;
    }
    else{
        pq->head = NULL;
        pq->tail = pq->head;
        return currLN;
    }
}

//todo
char* getListAsString(struct PlayerQueue* pq){
    return NULL;
}

void deletePlayerQueue(struct PlayerQueue* pq){
    struct ListNode *tmp = NULL, *ln = pq->head;
    while(ln != NULL){
        tmp = ln;
        ln = ln->next;
        free(tmp);
    }
    free(pq);
}

struct Player* createPlayer(int num){
    struct Player* p = (struct Player*) malloc(sizeof(struct Player));
    p->num = num;
    p->gun = DEFAULT_GUN;
    p->health = DEFAULT_HEALTH;
    p->queue = createPlayerQueue();
}

void deletePlayer(struct Player* p){
    deletePlayerQueue(p->queue);
    free(p);
}

struct Game* initGamestate() {
    struct Game* gameState = (struct Game*) malloc(sizeof(struct Game));
    gameState->gameover = false;
    gameState->gunlocked = false;
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        gameState->players[i] = createPlayer(i+1); 
    }
    return gameState;
}

void endGamestate(struct Game* g){
    for(int i = 0; i < NUM_OF_PLAYERS; i++){
        deletePlayer(g->players[i]);
    }
    free(g);
}

void addActionToPlayer(struct Game* g, int playerNum, char* action, int target){
    if(playerNum-1 < NUM_OF_PLAYERS){
        enqueueNewTask(g->players[playerNum-1], action, target);
    }
}

struct Action* getCurrentActionForPlayer(struct Game* g, int playerNum){
    if(playerNum-1 < NUM_OF_PLAYERS){
        struct Action* a = (struct Action*) malloc(sizeof(struct Action));
        struct ListNode* ln = dequeueCurrentTask(g->players[playerNum-1]);
        free(ln);
        a->action = ln->action;
        a->target = ln->target;
        return a;
    }
    return NULL;
}

// Applies the task and updates the queue of the player
void applyTask(struct Game* g, struct Player* p, struct Action* a){
    if(a->action == "attack"){
        g->players[a->target]->health -= 10;
        if(g->players[a->target]->health == 0){
            deletePlayer(g->players[a->target]);
        }
    } else if(a->action == "defense"){
        p->health += 10;
    } else if(a->action == "boost"){
        p->gun++;
    }
}

/*
void setSendSocketForPlayer(struct Game* g, int playerNum, int fd){
    if(playerNum > NUM_OF_PLAYERS){
        perror("index error: playerNum exceeds number of players");
        return;
    }
    g->players[playerNum]->sendSocket = fd;
}
int getSendSocketForPlayer(struct Game* g, int playerNum){
    if(playerNum > NUM_OF_PLAYERS){
        perror("index error: playerNum exceeds number of players");
        return -1;
    }
    return g->players[playerNum]->sendSocket;
}

void setRecieveSocketForPlayer(struct Game* g, int playerNum, int fd){
    if(playerNum > NUM_OF_PLAYERS){
        perror("index error: playerNum exceeds number of players");
        return;
    }
    g->players[playerNum]->recieveSocket = fd;
}
int getRecieveSocketForPlayer(struct Game* g, int playerNum){
    if(playerNum > NUM_OF_PLAYERS){
        perror("index error: playerNum exceeds number of players");
        return -1;
    }
    return g->players[playerNum]->recieveSocket;
}
*/

