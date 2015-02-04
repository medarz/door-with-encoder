/*
 * flash.h
 *
 *  Created on: 03/02/2015
 *      Author: Mario
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <msp430.h>

void write_flash_c(char, unsigned int);
void write_flash_i(int, unsigned int);
void erase_flash(void);
unsigned int current_ptr(void);


#endif /* FLASH_H_ */
