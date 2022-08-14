#ifndef clientGameDataStructures_h
#define clientGameDataStructures_h

// The client should know which id they are, as well as which id is their ally and enemies. The ID's range from 1 to 4.
struct PlayersInfo {
    /*
    int player; 
    int ally;   
    int enemy1; 
    int enemy2;
    */
    char player; 
    char ally;   
    char enemy1; 
    char enemy2;
};

#endif 