#include "gameStructures.h"

// creates PlayerQueue
struct PlayerQueue *createPlayerQueue()
{
    struct PlayerQueue *pq = (struct PlayerQueue *)malloc(sizeof(struct PlayerQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
}

// creates taskNode (used in enqueueNewTask, do not use elsewhere)
struct Action *createNode(char *action, int target)
{
    struct Action *act = (struct ListNode *)malloc(sizeof(struct Action));
    act->action = action;
    act->target = target;
    act->next = NULL;
    return act;
}

// adds new task to end of player queue
void enqueueNewTask(struct PlayerQueue *pq, char *action, int target)
{
    if (pq == NULL)
    {
        perror("PlayerQueue cannot be NULL");
        return;
    }

    struct Action *act = CreateNode(action, target);
    if (pq->head == NULL && pq->tail == NULL)
    {
        pq->head = act;
        pq->tail = pq->head;
    }
    else
    {
        pq->tail->next = act;
        pq->tail = act;
    }
    pq->size++;
}

// removes task from front of player queue
struct Action *dequeueCurrentTask(struct PlayerQueue *pq)
{
    if (pq == NULL)
    {
        perror("PlayerQueue cannot be NULL");
        return;
    }

    struct Action *currAction = pq->head;

    if (pq->head != pq->tail)
    {
        pq->head = pq->head->next;
        currAction->next = NULL;
        return currAction;
    }
    else
    {
        pq->head = NULL;
        pq->tail = pq->head;
        return currAction;
    }
}

// todo
char *getListAsString(struct PlayerQueue *pq)
{
    return NULL;
}

void deletePlayerQueue(struct PlayerQueue *pq)
{
    struct Action *tmp = NULL, *act = pq->head;
    while (act != NULL)
    {
        tmp = act;
        act = act->next;
        free(tmp);
    }
    free(pq);
}

struct Player *createPlayer(int num)
{
    struct Player *p = (struct Player *)malloc(sizeof(struct Player));
    p->num = num;
    p->gun = DEFAULT_GUN;
    p->health = DEFAULT_HEALTH;
    p->queue = createPlayerQueue();
    p->isBoostActive = false;
    p->boostCount = 0;
}

void deletePlayer(struct Player *p)
{
    deletePlayerQueue(p->queue);
    free(p);
}

struct Game *initGamestate()
{
    struct Game *gameState = (struct Game *)malloc(sizeof(struct Game));
    gameState->gameover = false;
    gameState->gunlocked = false;
    for (int i = 0; i < NUM_OF_PLAYERS; i++)
    {
        gameState->players[i] = createPlayer(i + 1);
    }
    return gameState;
}

void endGamestate(struct Game *g)
{
    for (int i = 0; i < NUM_OF_PLAYERS; i++)
    {
        deletePlayer(g->players[i]);
    }
    free(g);
}

void addActionToPlayer(struct Game *g, int playerNum, char *action, int target)
{
    if (playerNum - 1 < NUM_OF_PLAYERS)
    {
        enqueueNewTask(g->players[playerNum - 1], action, target);
    }
}

// If a player's next command is "gun boost" while another player is using the gun boost. That command is being ignored.
struct Action *getCurrentActionForPlayer(struct Game *g, int playerNum)
{
    if (playerNum - 1 < NUM_OF_PLAYERS)
    {
        for (int i = 0; i < NUM_OF_PLAYERS; i++)
        {
            if (g->players[i]->isBoostActive == true)
            {
                printf("Gun boost is unavailable!\nAnother player is using it.\nAdding the next command!");
                dequeueCurrentTask(g->players[playerNum - 1]->queue->head);
                getCurrentActionForPlayer(g, playerNum);
            }
            else
            {
                struct Action *a = (struct Action *)malloc(sizeof(struct Action));
                struct Action *act = dequeueCurrentTask(g->players[playerNum - 1]->queue->head);
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

void applyTask(struct Game *g, struct Player *p, struct Action *a)
{
    if (a->action == "attack")
    {
        if (p->isBoostActive)
        {
            g->players[a->target]->health -= 20;
            p->boostCount--;
            if (p->boostCount == 0)
            {
                p->isBoostActive = false;
            }
        }
        else
        {
            g->players[a->target]->health -= 10;
        }
        if (g->players[a->target]->health == 0)
        {
            deletePlayer(g->players[a->target]);
        }
    }
    else if (a->action == "defense")
    {
        p->health += 10;
    }
    else if (a->action == "boost")
    {
        p->gun++;
        if (p->gun == MAX_GUN_FILL)
        {
            p->isBoostActive = true;
            p->boostCount = BOOST_COUNT_START;
        }
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
