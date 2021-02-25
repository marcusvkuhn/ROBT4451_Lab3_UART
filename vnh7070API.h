/*
 * vnh7070API.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Raymond
 */

#ifndef VNH7070API_H_
#define VNH7070API_H_

#define CTRL_MASK 0x07
#define DS_DELAY 52500

char vnh7070InputCtrl (unsigned char ctrl);
char vnh7070CW (unsigned char dutyCyle);
char vnh7070CCW (unsigned char dutyCyle);
char vnh7070Brake();

#endif /* VNH7070API_H_ */
