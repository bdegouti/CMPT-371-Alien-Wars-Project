#include "PlayerQueue.h"

//creates PlayerQueue
PlayerQueue* createPlayerQueue(){
    PlayerQueue *pq = (PlayerQueue*) malloc(sizeof(PlayerQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
}

//creates taskNode (used in enqueueNewTask, do not use elsewhere)
ListNode* createNode(char* action, char* target) {
    struct ListNode *ls = (struct ListNode*) malloc(sizeof(ListNode));
    ls->action = action;
    ls->target = target;
    ls->next = NULL;
    return ls;
}

//adds new task to end of player queue
void enqueueNewTask (PlayerQueue* pq, char* action, char* target){
    if(pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return;
    }
    
    ListNode* ln = CreateNode(action, target);
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
ListNode* dequeueCurrentTask(PlayerQueue* pq){
    if(pq == NULL){
        perror("PlayerQueue cannot be NULL");
        return;
    }

    ListNode* currLN = pq->head;

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
char* getListAsString(PlayerQueue* pq){
    return NULL;
}

void deletePlayerQueue(PlayerQueue* pq){
    ListNode *tmp = NULL, *ln = pq->head;
    while(ln != NULL){
        tmp = ln;
        ln = ln->next;
        free(tmp);
    }
    free(pq);
}


