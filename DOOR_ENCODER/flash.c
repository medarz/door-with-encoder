/*
 * flash.c
 *
 *  Created on: 03/02/2015
 *      Author: Mario
 */

#include "flash.h"
#include "definitions.h"

unsigned int current_ptr_offset(unsigned char option){

	unsigned int i;

	if(option==1)
	{
		char *Flash_ptr;
		Flash_ptr = FLASH_SEG_D;             // Initialize Flash segment D ptr

		for (i = 0; i < 256; i++)            // Leer los dos primeros segmentos
		{
			if( Flash_ptr[i] == 0xFF)          // copy value segment C to seg D
				return i;
		}
	}
	else if(option==2)
	{
		int *Flash_ptr;
		Flash_ptr = FLASH_SEG_B;             // Initialize Flash segment D ptr

		for (i = 0; i < 64; i++)            // Leer los dos primeros segmentos
		{
			if( Flash_ptr[i] == 0xFFFF)          // copy value segment C to seg D
				return i;
		}
	}
	return 0;

}

void erase_flash(char *Flash_ptr){

	__disable_interrupt();

	if(Flash_ptr == (char *) 0x1980)
		FCTL3 = FWKEY+LOCKA;
	else
		FCTL3 = FWKEY; // Clear LOCK
	FCTL1 = FWKEY+ERASE; //Enable segment erase
	*Flash_ptr = 0; // Dummy write, erase Segment
	while(FCTL3 & BUSY);
	if(Flash_ptr == (char *) 0x1980)
		FCTL3 = FWKEY+LOCKA; // Done, set LOCK
	else
		FCTL3 = FWKEY+LOCK; // Done, set LOCK

	__enable_interrupt();

}

void write_flash_c(char value, unsigned int offset){


  char * Flash_ptr;                         // Initialize Flash pointer

  __disable_interrupt();

  Flash_ptr = FLASH_SEG_D;              	// Segment D
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation
  Flash_ptr[offset] = value;
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  __enable_interrupt();

}

void write_flash_i(int value, unsigned int offset)
{

  int * Flash_ptr;                         // Initialize Flash pointer

  __disable_interrupt();

  Flash_ptr = FLASH_SEG_B;              // Segment D
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  Flash_ptr[offset]   = (value);

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  __enable_interrupt();
}

void write_flash_segA(int value, unsigned char offset){

	  int * Flash_ptr;                         // Initialize Flash pointer

	  __disable_interrupt();

	  Flash_ptr = FLASH_SEG_A;              // Segment D
	  FCTL3 = FWKEY+LOCKA;                            // Clear Lock bit
	  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

	  Flash_ptr[offset]   = (value);

	  FCTL1 = FWKEY;                            // Clear WRT bit
	  FCTL3 = FWKEY+LOCKA;                       // Set LOCK bit

	  __enable_interrupt();
}

