#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

#define NUM_OF_PLAYERS 4
#define DEFAULT_GUN 0
#define DEFAULT_HEALTH 100

struct ListNode {
    char* action;
    int target;
    struct ListNode *next;
};

struct PlayerQueue {
    struct ListNode* head;
    struct ListNode* tail;
    int size;
};

struct Player {
    char* name;
    //int sendSocket, recieveSocket;
    struct PlayerQueue* queue;
    int health;
    int gun;
};

struct Game {
    bool gameover;
    struct Player* players[NUM_OF_PLAYERS];
    bool gunlocked;
};

struct Action {
    char* action;
    int target;
};

/*
* Allocate memory for PlayerQueue object.
* initialize with NULL values.
* return pointer to list.
*/
struct PlayerQueue* createPlayerQueue(){

}
/*
 * Allocate memory for a node of type struct ListNode and
 * initialize it with the item value.
 * Return a pointer to the new node.
 */
struct ListNode* createNode(char* action, int target);

/*
 * Insert node with specified item at the tail of the list.
 */
void enqueueNewTask (struct PlayerQueue* pq, char* action, int target);

/*
 *dequeue top node in list and return (with next set to NULL)
 */
struct ListNode* dequeueCurrentTask(struct PlayerQueue* pq);

/*
 * return the list as a string which can be sent to client
 */
char* getListAsString(struct PlayerQueue* pq);

/*
 * Delete PlayerQueue and release memory
 */
void deletePlayerQueue(struct PlayerQueue* pq);

/*
 * Creates and returns Player structure
 */
struct Player* createPlayer(char* name);

/*
 *deletes player structure and frees memory in self and substructures
 */
void deletePlayer(struct Player* p);

/*
 *initializes gamestate and sets up empty player structures
 */
struct Game* initGameState();

/*
 *ends game and frees used memory in self and all substructures
 */
void endGameState(struct Game* g);

void setSendSocketForPlayer(struct Game* g, int playerNum, int fd);
int getSendSocketForPlayer(struct Game* g, int playerNum);

void setRecieveSocketForPlayer(struct Game* g, int playerNum, int fd);
int getRecieveSocketForPlayer(struct Game* g, int playerNum);

/*
 * adds action to the player at the given index
 */
void addActionToPlayer(struct Game* g, int playerNum, char* action, int target);

/*
 * gets the action of a player at the top of their queue
 */
struct Action* getCurrentActionForPlayer(struct Game* g, int playerNum);

