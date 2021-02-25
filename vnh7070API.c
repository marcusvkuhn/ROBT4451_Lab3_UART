#include <vnh7070API.h>
#include <msp430.h>
#include <pwmTimerA0.h>
/*
 * vnh7070API.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Raymond
 */
#define IN_A BIT0
#define IN_B BIT1
#define SEL BIT2
#define CW_CTRL 0x01
#define CCW_CTRL 0x02

char vnh7070InputCtrl (unsigned char ctrl){
    volatile int validInputCtrl = -1;
    if(ctrl <= CTRL_MASK){               /* Raw vnh7070 Input control.   INa, INb, SEL.  ctrl can be in range 0x0 -> 0x7.
                                         *
                                         * INa = ctrl<0>, INb = ctrl<1>,  SEL = ctrl<2>
                                         * If ctrl is in range and is output, then 0 is returned otherwise -1 is returned. */
        P3DIR |= CTRL_MASK;
        validInputCtrl = 0;
    }
    return validInputCtrl;
}
char vnh7070CW (unsigned char dutyCycle){
    volatile int validCWctrl = -1, validCW = -1, validCWdutyCycle = -1, i;
    unsigned char ctrlCW = 0;

    validCWctrl = vnh7070InputCtrl(P3OUT & CTRL_MASK);
    validCWdutyCycle = timerA0DutyCycleSet(dutyCycle);

    if(!validCWctrl & !validCWdutyCycle){
        if(P3OUT & IN_B){   // checks if direction is different
            // de accel
           for(i = 1; i < dutyCycle; i++){
               timerA0DutyCycleSet(dutyCycle - i);
               __delay_cycles(DS_DELAY);          // 50ms delay per duty cycle change
           }
            // switch directions
            validCWctrl = vnh7070InputCtrl(CW_CTRL);
            if(!validCWctrl)
                P3OUT = CW_CTRL;
            // accelerate
            for (i = 0; i <= dutyCycle; i++){
                timerA0DutyCycleSet(i);
                __delay_cycles(DS_DELAY);          // 50ms delay per duty cycle change
            }
        }
        else{
            validCWctrl = vnh7070InputCtrl(ctrlCW);
            if(!validCWctrl)
                P3OUT = CW_CTRL;
        }
        P3OUT &= ~SEL;
        validCW = 0;
    }
    return validCW;

    /* Executes CW control signals   with requested duty cycle to vnh7070.
     * Calls vnh7070 InputCtrl,  timerA0DutyCycle
     * SetSets SEL LOW.
     * Returns 0 if successful, -1 otherwise.      */
}
char vnh7070CCW (unsigned char dutyCycle){
    volatile int validCCWctrl = -1, validCCW = -1, validCCWdutyCycle = -1, i;
    unsigned char ctrlCCW = 0;

    validCCWctrl = vnh7070InputCtrl(P3OUT & CTRL_MASK);
    validCCWdutyCycle = timerA0DutyCycleSet(dutyCycle);

    if(!validCCWctrl & !validCCWdutyCycle){
        if(P3OUT & IN_A){   // checks if direction is different
            // de accel
           for(i = 1; i < dutyCycle; i++){
               timerA0DutyCycleSet(dutyCycle - i);
               __delay_cycles(DS_DELAY);          // 50ms delay per duty cycle change
           }
            // switch directions
            validCCWctrl = vnh7070InputCtrl(CCW_CTRL);
            if(!validCCWctrl)
                P3OUT = CCW_CTRL;
            // accelerate
            for (i = 0; i <= dutyCycle; i++){
                timerA0DutyCycleSet(i);
                __delay_cycles(DS_DELAY);          // 50ms delay per duty cycle change
            }
        }
        else{
            validCCWctrl = vnh7070InputCtrl(ctrlCCW);
            if(!validCCWctrl)
                P3OUT = CCW_CTRL;
        }
        P3OUT &= ~SEL;
        validCCW = 0;
    }
    return validCCW;

    /*
     * Executes CCW control signals with requested duty cycle to vnh7070.
     * Calls vnh7070InputCtrl,  timerA0DutyCycleSet
     * Sets SEL LOW.
     * Returns 0 if successful, -1 otherwise.
     */
}
char vnh7070Brake(){
    volatile int validBrakeCtrl = -1, validBrakeDutyCycle = -1, validBrake = -1;
    unsigned char ctrlBrake = 0;

    ctrlBrake &= ~(IN_A + IN_B);

    validBrakeCtrl = vnh7070InputCtrl(ctrlBrake);

    validBrakeDutyCycle = timerA0DutyCycleSet(10);

    if (!validBrakeCtrl & !validBrakeDutyCycle){
        P3OUT &= ~SEL;
        validBrake = 0;
    }

    return validBrake;
}
