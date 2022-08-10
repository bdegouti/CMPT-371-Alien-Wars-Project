#include "userAction.h"

// enum for console colors
enum theme
{
    white,
    red,
    blue,
    green
};

// theme codes for console colors
const char *THEME_CODES[5] = {"\033[37m",
                              "\033[31m",
                              "\033[34m",
                              "\033[32m"};
const char *CLEAR_CODE = "\033[0m"; // clears all console formatting

const char INPUT_ERROR[] = "Please enter a valid move!\nMoves are att, def, gun\n\n";
const char TARGET_ERROR[] = "\033[31mInvalid Player Target!\033[0m\nValid targets for %s are %c and %c\n";

// gets user input
// checks for user input error
// returns valid user move
char *getUserAction(struct PlayersInfo playersInfo)
{
    char buffer[7];                // input size of 5 + \n + \0
    char *ret = (char *)malloc(6); // 5 + \0

    // loop till input valid
    bool input_not_valid = true;
    while (input_not_valid)
    {
        // get user input
        printf("%s<move> <player target> %sEnter your move: %s", THEME_CODES[blue], THEME_CODES[green], THEME_CODES[red]);
        fgets(buffer, 7, stdin);
        buffer[5] = '\0';         // remove the \n from string
        printf("%s", CLEAR_CODE); // clear text formatting

        // check user input valid
        if (strncmp(buffer, ATT, 3) == 0)
        { // Attack
            if (buffer[4] == playersInfo.enemy1 || buffer[4] == playersInfo.enemy2)
            {
                strcpy(ret, buffer);
                input_not_valid = false;
            }
            else
            {
                printf(TARGET_ERROR, ATT, playersInfo.enemy1, playersInfo.enemy2);
                continue;
            }
        }
        else if (strncmp(buffer, DEF, 3) == 0)
        { // Defense
            if (buffer[4] == playersInfo.player || buffer[4] == playersInfo.ally)
            {
                strcpy(ret, buffer);
                input_not_valid = false;
            }
            else
            {
                printf(TARGET_ERROR, DEF, playersInfo.player, playersInfo.ally);
                continue;
            }
        }
        else if (strncmp(buffer, GUN, 3) == 0)
        { // Loading Gun
            strcpy(ret, GUN);
            ret[3] = ' ';
            ret[4] = playersInfo.player;
            ret[5] = '\0';
            input_not_valid = false;
        }

        if (input_not_valid)
        {
            printf("%s%s%s", THEME_CODES[red], INPUT_ERROR, CLEAR_CODE); // print error and moves
        }
    }

    return ret;
}