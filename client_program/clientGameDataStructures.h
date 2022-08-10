// Defines the playersInfo Struct which will be sent to the client by the server. 

#ifndef clientGameDataStructures_h
#define clientGameDataStructures_h


#include <stdbool.h>


// The client should know which id they are, as well as which id is their ally and enemies. 
// The ID's range from 1 to 4. For now I have them as char, but this could change.
struct PlayersInfo {
    int player; 
    int ally;   
    int enemy1; 
    int enemy2;
};

// GameState Recieved from Server 
struct GameState {
    bool gameOver;
    char* gameOverMsg; 
};


#endif 