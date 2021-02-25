/*
 * timerA0.h
 *
 *  Created on: Jan 7, 2021
 *      Author: a01064898
 */

#ifndef PWMTIMERA0_H_
#define PWMTIMERA0_H_

#define SMCLKFREQ 1048000
#define RAMP_DELAY 52500

unsigned int timerA0Init(unsigned int pwmFreq);
char timerA0PwmFreqSet(unsigned int pwmFreq);
char timerA0DutyCycleSet(unsigned char dutyCycle);

volatile int currentDS;

#endif /* PWMTIMERA0_H_ */
