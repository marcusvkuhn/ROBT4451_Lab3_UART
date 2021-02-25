#include <cmdVNH7070.h>
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwmTimerA0.h>
#include <vnh7070API.h>
#include <ctype.h>

/************************************************************************************
* Function: initVnhCmd
* Purpose: initialize each index of the array with its correspondent string and nArgs
* arguments:
*   vnh7070Cmds   -   CMD
* return:  none
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
void initVnh7070Cmds(CMD* vnh7070Cmds){
    vnh7070Cmds[0].name = CMD0;        // initialize the first command name
    vnh7070Cmds[0].nArgs = CMD0_NARGS; // initialize num of arguments in first command
    vnh7070Cmds[1].name = CMD1;
    vnh7070Cmds[1].nArgs = CMD1_NARGS;
    vnh7070Cmds[2].name = CMD2;
    vnh7070Cmds[2].nArgs = CMD2_NARGS;
}

/************************************************************************************
* Function: parseCmd
* Purpose: splits strings into separate tokens for speceific porpuses
* arguments:
*   vnh7070Cmds   -   CMD
*   cmdLine   -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int parseCmd(CMD* vnh7070Cmds, char* cmdLine){
    char* token;  // a pointer variable to a string.
    //char* con;
    int i = 0;

    // token is assigned the pointer to the token returned by strtok_s
    token = strtok(cmdLine, DELIM);

    int cmdIndex = validateCmd(vnh7070Cmds, token);     // finds which index to use for subsequent parsing

    if (cmdIndex != -1 && vnh7070Cmds[cmdIndex].nArgs == 0){
        token = strtok(NULL, DELIM);
        if (token)     // if there is more than one argument, function will also return -1
            cmdIndex = -1;
    }
    if (cmdIndex != -1 && vnh7070Cmds[cmdIndex].nArgs != 0){
        if (cmdIndex == PWMFREQSET_IDX){      // for scaraMotorSpeed, nArgs = 1
                token = strtok(NULL, DELIM);
                // checks if token exists, is of lenght 1 and is alphabetic
                if (token && isdigit((int)*token))
                    ARGS[0] = (int)token;            // casts as integer the value (of a char) token is pointing to
                else
                    cmdIndex = -1;      //
                //if(!token)                 // if token is NULL, command is invalid, function will return -1
                //    cmdIndex = -1;
                token = strtok(NULL, DELIM);
                if (token)     // if there is more than one argument, function will also return -1
                    cmdIndex = -1;
        }
        if (cmdIndex == VNHMOVE_IDX){      // for scaraMoveJ, nArgs = 2, for scaraMoveL, nArgs = 5, for scaraPenColor, nArgs = 3
            for (i = 0; token && i < vnh7070Cmds[cmdIndex].nArgs; ++i){      // loops while token is valid and i is less than the nArgs for the command, same for every following for-loop
                token = strtok(NULL, DELIM);
                if (token)                                              // every following token here will be a double
                    ARGS[i] = atof(token);            // atof converts to double, then it is stored into the array
                else if (!token)                // token invalid in the range of nArgs
                    cmdIndex = -1;
            }
            if (token = strtok(NULL, DELIM))     // looks for more than nArgs arguments, which will also make the function return -1
                cmdIndex = -1;
        }
    }
    return cmdIndex;
}

/************************************************************************************
* Function: validadeCmd
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdName     -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int validateCmd(CMD* vnh7070Cmds, char* cmdName){
    unsigned int i = 0;
    int idx = -1;
    int invalidCmd = 1;
    while (invalidCmd && i < MAX_CMDS)
        invalidCmd = strcmp(cmdName, vnh7070Cmds[i++].name);
    if (!invalidCmd)
        idx = i - 1;
    return idx;
}

/************************************************************************************
* Function: executeCMD
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdIndex   -   int
*   scaraState   -   SCARA_ROBOT*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int executeCMD(CMD* vnh7070Cmds, int cmdIndex){
    switch (cmdIndex)
    {
    case PWMFREQSET_IDX:
        timerA0PwmFreqSet(vnh7070Cmds->args[0]);
        break;
    case VNHMOVE_IDX:
        if(vnh7070Cmds->args[0])
            vnh7070CW(vnh7070Cmds->args[1]);
        else if(!vnh7070Cmds->args[0])
            vnh7070CCW(vnh7070Cmds->args[1]);
        break;
    case VNHBRAKE_IDX:
        vnh7070Brake();
        break;
    default:
        break;
    }
    return cmdIndex;
}
