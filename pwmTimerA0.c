#include <msp430.h>
#include <pwmTimerA0.h>

/************************************************************************************
  * Function: timerA0Init
  * - Initializes timer A0, sets period, selects clock source, clock dividers,
  * UP mode, clears main clock;
  * - Muxes timer signal to P1.2.
  *
  * Arguments: none
  *
  * return: none
  *
  * Author: Marcus Kuhn
  * Date: 1/6/2021
  * Modified: 1/7/2021
  ***********************************************************************************/
unsigned int timerA0Init(unsigned int pwmFreq){
    volatile int validFreqInit = -1;

    validFreqInit = timerA0PwmFreqSet(pwmFreq);

    if(!validFreqInit){
        TA0CTL = TASSEL_2 | ID__1 | MC_1 | TACLR; // source select: SMCLK | input divider: 1 | mode control: UP | clear clk
        // clock divisor is now 1
        TA0EX0 |= TAIDEX_0;    // expansion clk divider to 1
        TA0CCTL1 |= OUTMOD_7;     // set to output mode

        // set TA0.1 to be output @ P1.2
        P1DIR |= BIT2;
        P1SEL |= BIT2;

        timerA0DutyCycleSet(0);
    }

    return validFreqInit;
}


char timerA0PwmFreqSet(unsigned int pwmFreq){
    volatile int m, validPwmFreq = -1;
    volatile double period;

    m = (int)(SMCLKFREQ/pwmFreq);


    if(m >= 9 && m <= 65356){
        TA0CCR0 = m - 1;
        validPwmFreq = 0;
    }
    return validPwmFreq;
}

char timerA0DutyCycleSet (unsigned char dutyCycle){
    volatile int validDS = -1, i;
    volatile double n = 0;

    if(dutyCycle > 0 && dutyCycle < 11){
        if(dutyCycle - currentDS > 4)
            for(i = currentDS; i <= dutyCycle; i++){
                n = (i * 0.1);
                TA0CCR1 = (int)(TA0CCR0 * n);
                __delay_cycles(RAMP_DELAY);          // 50ms delay per duty cycle change
            }
        else
            n = (dutyCycle * 0.1);
            TA0CCR1 = (int)(TA0CCR0 * n);
            currentDS = dutyCycle;
            validDS = 0;
    }
    return validDS;
}
