/*************************************************************************************************
 * usciUart.c
 * - C implementation or source file for MSP430 usci UART A1
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *  Modified: February 26th, 2018
 **************************************************************************************************/

#include <msp430.h>
#include "usciUart.h"
#include "stdio.h"



/************************************************************************************
* Function: usciA1UartInit
* - configures UCA1 UART to use SMCLK, no parity, 8 bit data, LSB first, one stop bit
*  BAUD rate = 19.2Kbps with 16xoversampling.
*  assumes SMCLK = 2^20 Hz.
* argument:
* Arguments: none, but baud rate would be useful
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartInit(){

	// READ THIS --> You must use usci Control BITS defined in msp430.h !!
	// In fact you must use pre-defined control bits whenever possible. no obscure HEX codes allowed anymore.

	P4SEL |= TXD_A1 | RXD_A1;
	UCA1CTL1 |= UCSWRST; 			// USCI A1  Software Reset Enabled
	//********************

	UCA1CTL1 	|= 	UCSSEL_2; 		// select SMCLK. User is responsible for setting this rate.

	UCA1CTL0	= 	0; 				// RESET UCA1CTL0 before new configuration
	UCA1CTL0	&=	~UCPEN			// No Parity
				&	~UCMSB			// LSB First
				&	~UC7BIT			// 8 bits of data
				&	~UCSPB			// 1 stop bit
				&	~UCSYNC;		// UART Mode


	UCA1BR0 = 0x03; // calculate and confirm with table
	UCA1MCTL = UCBRF_6 + UCBRS1 + UCOS16; // calculate and confirm with table.


	UCA1CTL1 	&= ~UCSWRST; 		//  configured. take state machine out of reset.
	}


/************************************************************************************
* Function: usciA1UartTxChar
* - writes a single character to UCA1TXBUF if it is ready
* argument:
* Arguments: txChar - byte to be transmitted
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartTxChar(unsigned char txChar) {

	while (!(UCA1IFG & UCTXIFG)); // is this efficient ?
		UCA1TXBUF = txChar;	 // if TXBUFF ready then transmit a byte by writing to it
}


/************************************************************************************
* Function: usciA1UartTxString
* - writes a C string of characters, one char at a time to UCA1TXBUF by calling
*   usciA1UartTxChar. Stops when it encounters  the NULL character in the string
*   does NOT transmit the NULL character
*
* Arguments: txChar - pointer to char (string) to be transmitted
*
* return: number of characters transmitted
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartTxString(unsigned char *txChar){
	// while not a null character
    while(*txChar != NULL_CHAR){
	// transmit a character with usciA1UartTxChar and increment the pointer
        usciA1UartTxChar(*txChar++);
        }
	}

int usciA1UartTxBuffer(unsigned char* buffer, int buffLen){
    volatile int i = 0;

    for(i = 0; (i < buffLen) && (*buffer != NULL_CHAR) ; i++){
            usciA1UartTxChar(*buffer++);
    }
    return i;
}

/************************************************************************************
* Function: usciA1UartTxString
* - writes a C string of characters, one char at a time to UCA1TXBUF by calling
*   usciA1UartTxChar. Stops when it encounters  the NULL character in the string
*   does NOT transmit the NULL character
*
* Arguments: txChar - pointer to char (string) to be transmitted
*
* return: number of characters transmitted
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
char* usciA1UartGets(char* rxString){
    unsigned int i = 0;
//    unsigned int validGets = 1;

    do{
       while(!(UCA1IFG & UCRXIFG));
       rxBuffer[i++] = UCA1RXBUF;
       usciA1UartTxChar(UCA1RXBUF);
    }while((UCA1RXBUF != NL_CHAR) && (i < BUFF_SZ));

//    if (i >= BUFF_SZ){
//        validGets = 0;
//    }
//    else{
        rxBuffer[i] = NULL_CHAR;
        strcpy(rxString,rxBuffer);
        usciA1UartTxChar('\n');
//    }
    return rxString;
}

#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;
  case 2:
      // 5.6.1
      //if(UCTXIFG)
	  // UCA1TXBUF = UCA1RXBUF;
    break;
  case 4:break;
  default: break;
  }
}


