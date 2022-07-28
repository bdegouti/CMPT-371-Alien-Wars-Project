#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

//enum for console colors
enum theme
{
    white,
    red,
    blue,
    green
};

//theme codes for console colors
const char *THEME_CODES[5] = {"\033[37m",
                              "\033[31m",
                              "\033[34m",
                              "\033[32m"};
const char *CLEAR_CODE = "\033[0m"; //clears all console formatting

const char INPUT_ERROR[] = "Please enter a valid move!\nMoves are att, def, gun\n\n";
const char *MOVES[] = {"att",
                       "def",
                       "gun"};

bool gameOver = false; //true when game over
char *gunCommand = "gun 1"; // ``` Temporary placeholder ``` ///

//gets user input and deals with error checking
//remember to free the returned string
char *getUserInput()
{
    char buffer[7]; // input size of 5 + \n + \0
    char *ret = (char *)malloc(6);

    while (1)
    {
        printf("%s<move> <player target> %sEnter your move: %s", THEME_CODES[blue], THEME_CODES[green], THEME_CODES[red]);

        fgets(buffer, 7, stdin);
        buffer[5] = '\0';
        printf("%s", CLEAR_CODE); // clear text formatting
        for (int i = 0; i < 3; i++)
        {
            if (strncmp(buffer, MOVES[i], 3) == 0)
            {
                if (buffer[4] >= 49 && buffer[4] <= 52)
                {
                    strcpy(ret, buffer);
                    return ret;
                }
                else
                {
                    if (strncmp(buffer, MOVES[2], 3) == 0)
                    {
                        strcpy(ret, gunCommand);
                        return ret;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        printf("%s%s%s", THEME_CODES[red], INPUT_ERROR, CLEAR_CODE);
    }
}

int main(int argc, char *argv[])
{
    while (!gameOver)
    {
        char *ret = getUserInput(); //Remember to free !!
        printf("%s is a valid move\n", ret);
        free(ret);
        ret = NULL;
    }
}