#include <stdio.h>
#include <string.h>


struct PlayerAction {
    int player;
    char* action;
    int targetPlayer;
};

// Converts clientSocketId (1..4) and clientActionMsg("move targetPlayer") into PlayerAction struct. 
struct PlayerAction getPlayerAction(int clientSocketId, char * clientActionMsg) {
    struct PlayerAction playerAction;

    playerAction.player = clientSocketId;

    // Matching Target Player 
    if (clientActionMsg[4] == '1') {
        playerAction.targetPlayer = 1;
    } else if (clientActionMsg[4] == '2') {
        playerAction.targetPlayer = 2;
    } else if (clientActionMsg[4] == '3') {
        playerAction.targetPlayer = 3;
    } else if (clientActionMsg[4] == '4') {
        playerAction.targetPlayer = 4;
    }

    // Matching Action
    if(strncmp(clientActionMsg, "att", 3) == 0) {
        playerAction.action = "att";
    } else if(strncmp(clientActionMsg, "def", 3) == 0) {
        playerAction.action = "def";
    } else if(strncmp(clientActionMsg, "gun", 3) == 0) {
        playerAction.action = "gun";
        playerAction.targetPlayer = playerAction.player; 
    }

    return playerAction;
}

// This function is for testing
void print_PlayerAction(struct PlayerAction playerAction) {
    printf("%d ", playerAction.player);
    printf("%s ", playerAction.action);
    printf("%d \n", playerAction.targetPlayer);
}


// This main function is for testing
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