/*************************************************************************************************
 * main_lab.c
 * - top level file with main{} for UART lab3
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2018
 **************************************************************************************************/

#include <msp430.h> 
#include "usciUart.h"
#include "cmdVNH7070.h"
#include "pwmTimerA0.h"
#include "vnh7070API.h"

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    timerA0Init(20000);

    //int txChars = 0;
    //char str[80];
    //unsigned char testChar[] = "BCIT MECHATRONICS\n";
    char rxString[50];
    unsigned char errorMsg[] = "Error!";
    CMD vnh7070Cmds[MAX_CMDS]; //this is an array of vnh7070Cmds of type CMD
    initVnh7070Cmds(vnh7070Cmds);


    usciA1UartInit();
    int cmdIndex;
        do{
            usciA1UartGets(rxString);
            cmdIndex = parseCmd(vnh7070Cmds, rxString);
            if (cmdIndex != -1){
                if (cmdIndex != QUIT_IDX){
                    //displayParsing(scaraCmds, cmdIndex);
                    executeCMD(vnh7070Cmds, cmdIndex);
                }
            }
            else
                usciA1UartTxString("Invalid command.");
        } while (cmdIndex != QUIT_IDX);
    // interrupts
    //UCA1IE |= UCRXIE;
    //__enable_interrupt();

    while (1){

//    	usciA1UartTxString(testChar);
//    	txChars = usciA1UartTxBuffer(testChar, 9);
//    	sprintf(str, "\n%d characters transmitted\n", txChars);
//    	usciA1UartTxString(str);
        if(!usciA1UartGets(rxString))
            usciA1UartTxString(errorMsg);

    }
}
