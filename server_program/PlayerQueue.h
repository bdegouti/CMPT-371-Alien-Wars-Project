// Linked list header file
#include <stdlib.h>
#include <string.h>

#ifndef LIST_H_
#define LIST_H_

typedef struct ListNode {
    char* action;
    char* target;
    struct ListNode *next;
} ListNode;

typedef struct PlayerQueue {
    ListNode* head;
    ListNode* tail;
    int size;
} PlayerQueue;

/*
* Allocate memory for PlayerQueue object.
* initialize with NULL values.
* return pointer to list.
*/
PlayerQueue* createPlayerQueue(){

}
/*
 * Allocate memory for a node of type struct ListNode and
 * initialize it with the item value.
 * Return a pointer to the new node.
 */
ListNode* createNode(char* action, char* target);

/*
 * Insert node with specified item at the tail of the list.
 */
void enqueueNewTask (PlayerQueue* pq, char* action, char* target);

/*
 *dequeue top node in list and return (with next set to NULL)
 */
struct ListNode* dequeueCurrentTask(PlayerQueue* pq);

/*
 * return the list as a string which can be sent to client
 */
char* getListAsString(PlayerQueue* pq);

/*
 * Delete PlayerQueue and release memory
 */
void deletePlayerQueue(PlayerQueue* pq);

#endif
