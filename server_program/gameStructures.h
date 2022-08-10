#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h> 

#define BUFFER_SIZE 8192 //size of buffer (subject to change)
#define SMALL_BUFFER 10
#define NUM_OF_PLAYERS 4
#define DEFAULT_GUN 0
#define DEFAULT_HEALTH 100
#define MAX_GUN_FILL 5
#define BOOST_COUNT_START 5
#define BOOST_COUNT_END 5

struct PlayerQueue {
    struct Action* head;
    struct Action* tail;
    int size;
};

struct Player {
    int num;
    struct PlayerQueue* queue;
    int health;
    int gun;
    bool isBoostActive;
    int boostCount;
};

struct Game {
    bool gameover;
    struct Player* players[NUM_OF_PLAYERS];
    bool gunlocked;
};

struct Action {
    char* action;
    int target;
    struct Action *next;
};

/*
* Allocate memory for PlayerQueue object.
* initialize with NULL values.
* return pointer to list.
*/
struct PlayerQueue *createPlayerQueue();
/*
 * Allocate memory for a node of type struct ListNode and
 * initialize it with the item value.
 * Return a pointer to the new node.
 */
struct Action* createNode(char* action, int target);

/*
 * Insert node with specified item at the tail of the list.
 */
void enqueueNewTask (struct PlayerQueue* pq, char* action, int target);

/*
 *dequeue top node in list and return (with next set to NULL)
 */
struct Action* dequeueCurrentTask(struct PlayerQueue* pq);

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
struct Player* createPlayer(int name);

/*
 *deletes player structure and frees memory in self and substructures
 */
void deletePlayer(struct Player* p);

/*
 *initializes gamestate and sets up empty player structures
 */
struct Game* initGameState();

/*
 * returns array representing play order for the round
 */
int* randomizePlayOrder();
/*
Executes a single round of the Game
*/
void executeRound(struct Game* g);

/*
 *ends game and frees used memory in self and all substructures
 */
void endGameState(struct Game* g);

/*
 * adds action to the player at the given index
 */
void addActionToPlayer(struct Game* g, int playerNum, char* action, int target);

/*
 * gets the action of a player at the top of their queue
 */
struct Action* getCurrentActionForPlayer(struct Game* g, int playerNum);

/*
 * executes the task at the top of a player's queue
 */
void applyTask(struct Game *g, struct Player *p, struct Action *a);

/*
 * return the winner of the game
 */
int whoWon(struct Game *g);


/*
 * returns stringified game state
 */
char *getGameStateAsString(struct Game *g);
