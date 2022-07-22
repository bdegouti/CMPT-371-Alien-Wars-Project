#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

const char INPUT_ERROR []= "Please enter a valid move!\n\n";
const char *MOVES[] = {"att",
                    "def",
                    "gun"};

bool gameOver = false;
char * gunCommand = "gun 1";

char * getUserInput() {
    char buffer[6]; //max input is 5 + '\0'
    char * ret = (char *)malloc(6);
    while(1) {
        printf("Moves are att, def, gun\n");
        printf("<move> <player target> Enter your move: ");
        fgets(buffer, 6, stdin);
        
        for(int i = 0; i < 3; i++) {
            if(strncmp(buffer, MOVES[i], 3) == 0) {
                if(buffer[4] >= 49 && buffer[4] <= 52) {
                    strcpy(ret, buffer);
                    return ret;    
                } else {
                    if(strncmp(buffer, MOVES[2], 3) == 0) {
                        strcpy(ret, gunCommand);
                        return ret;
                    } else {
                        printf(INPUT_ERROR);
                        continue;
                    }
                }
            }
        }
        printf(INPUT_ERROR);
    }
}

int main(int argc, char *argv[]) {


    while(!gameOver) {
        char * ret = getUserInput();
        printf("%s is a valid move\n", ret);
    }
}