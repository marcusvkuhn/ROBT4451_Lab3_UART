/*
 * cmdVNH7070.h
 *
 *  Created on: Feb 23, 2021
 *      Author: Raymond
 */

#ifndef CMDVNH7070_H_
#define CMDVNH7070_H_

#define     MAX_CMDS             3

#define     CMD0                 "pwmFreqSet"
#define     CMD0_NARGS           1
#define     PWMFREQSET_IDX       0

#define     CMD1                 "vnhMove"
#define     CMD1_NARGS           2
#define     VNHMOVE_IDX          1

#define     CMD2                 "vnhBrake\015"
#define     CMD2_NARGS           0
#define     VNHBRAKE_IDX         2

#define     DELIM                " ,\t"
#define     NULL                 '\0'          // null char
#define     ARGS                 vnh7070Cmds[cmdIndex].args
#define     MAX_ARGS             2

#define     QUIT_IDX             6

typedef struct CMD {
    const char *name; // command name
    int nArgs; // number of input arguments for a command
    int args[MAX_ARGS]; // arguments
}CMD;

//-------------- func prototypes-------------

void initVnh7070Cmds(CMD* vnh7070Cmds);
int parseCmd(CMD* vnh7070Cmds, char* cmdLine);
int validateCmd(CMD* scaraCmds, char* cmdName);
int executeCMD(CMD* vnh7070Cmds, int cmdIndex);

#endif /* CMDVNH7070_H_ */
