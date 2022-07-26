// do not
// write server code
// in this file
// this is for struct example

#include <stdio.h>

typedef struct Player {
  char* name;
  int health;
  int gun;
}player;

void attFucntion (player* p){
    p->health = p->health - 20;
}

int main () {
    // default states
    player p1,p3;
    p1.name = "P1";
    p1.health = 100;
    p1.gun = 0;
    
    p3.name = "P3";
    p3.health = 100;
    p3.gun = 0;

    
    //game logic
    printf("before p3 health = %d\n", p3.health);
    attFucntion(&p3);
    printf("after p3 health = %d\n", p3.health);

    return 0;
}