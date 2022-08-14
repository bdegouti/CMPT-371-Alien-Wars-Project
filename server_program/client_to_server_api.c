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
