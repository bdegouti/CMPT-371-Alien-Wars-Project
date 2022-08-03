// This file is an example of a player structure. 
// This code should not be used in the real program. 

#include <stdio.h>

// An example of a player struct 
struct Player {
    char* name;
    int health;
    int gun;
};

// example of a game logic function
void attackPlayer (struct Player* p){
    p->health = p->health - 20;
}

int main () {
    // Game State Initialization
    struct Player p1, p3;

    p1.name = "P1";
    p1.health = 100;
    p1.gun = 0;
    
    p3.name = "P3";
    p3.health = 100;
    p3.gun = 0;
    
    // Sample Game Logic - Attacking a Player
    printf("before p3 health = %d\n", p3.health);
    attackPlayer(&p3);
    printf("after p3 health = %d\n", p3.health);

    return 0;
}