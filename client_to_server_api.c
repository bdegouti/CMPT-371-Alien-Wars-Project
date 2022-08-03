#include <stdio.h>
#include <string.h>

enum Action {Attack, Defend, Gun};
enum Player {One, Two, Three, Four, Null};

struct PlayerAction {
    enum Player player;
    enum Action action;
    enum Player targetPlayer;
};

// Converts clientSocketId (1..4) and clientActionMsg("move targetPlayer") into PlayerAction struct. 
struct PlayerAction getPlayerAction(int clientSocketId, char * clientActionMsg) {
    struct PlayerAction playerAction;

    // Matching Player 
    if (socket_id == 1) {
        playerAction.player = One; 
    } else if (socket_id == 2) {
        playerAction.player = Two; 
    } else if (socket_id == 3) {
        playerAction.player = Three; 
    } else if (socket_id == 4) {
        playerAction.player = Four; 
    }

    // Matching Target Player 
    if (userMsg[4] == '1') {
        playerAction.targetPlayer = One;
    } else if (userMsg[4] == '2') {
        playerAction.targetPlayer = Two;
    } else if (userMsg[4] == '3') {
        playerAction.targetPlayer = Three;
    } else if (userMsg[4] == '4') {
        playerAction.targetPlayer = Four;
    }

    // Matching Action
    if(strncmp(userMsg, "att", 3) == 0) {
        playerAction.action = Attack;
    } else if(strncmp(userMsg, "def", 3) == 0) {
        playerAction.action = Defend;
    } else if(strncmp(userMsg, "gun", 3) == 0) {
        playerAction.action = Gun;
        playerAction.targetPlayer = playerAction.player; 
    }

    return playerAction;
}

void print_PlayerAction(struct PlayerAction playerAction) {
    switch (playerAction.player) {
        case One: printf("One"); break;
        case Two: printf("Two"); break;
        case Three: printf("Three"); break;
        case Four: printf("Four"); break;
    };
    printf(" ");

    switch (playerAction.action) {
        case Attack: printf("Attack"); break;
        case Defend: printf("Defend"); break;
        case Gun: printf("Gun"); break;
    };
    printf(" ");

    switch (playerAction.targetPlayer) {
        case One: printf("One"); break;
        case Two: printf("Two"); break;
        case Three: printf("Three"); break;
        case Four: printf("Four"); break;
    };
    printf("\n");
}


int main () {
    // This will be called inside a socket listen, and be passed to gamelogic. 
    struct PlayerAction pa1 = getPlayerAction(2, "att 3"); 
    struct PlayerAction pa2 = getPlayerAction(2, "def 1"); 
    struct PlayerAction pa3 = getPlayerAction(3, "gun"); 
    struct PlayerAction pa4 = getPlayerAction(3, "att 2"); 
    struct PlayerAction pa5 = getPlayerAction(1, "def 1"); 

    print_PlayerAction(pa1);
    print_PlayerAction(pa2);
    print_PlayerAction(pa3);
    print_PlayerAction(pa4);
    print_PlayerAction(pa5);

    return 0;
}