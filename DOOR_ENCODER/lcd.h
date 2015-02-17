/*
 * lcd16.h
 *
 *  Created on: Dec 1, 2011
 *      Author: Gaurav www.circuitvalley.com
 */

#ifndef LCD16_H_
#define LCD16_H_


#include <msp430.h>
#define  EN BIT5
#define  RS BIT6

void waitlcd(unsigned int x);
void lcdinit();
void lcdclear(unsigned char x);
void integerToLcd(int integer );
void lcdData(unsigned char l);
void prints(char *s);
void gotoXy(unsigned char  x,unsigned char y);

#endif /* LCD16_H_ */
