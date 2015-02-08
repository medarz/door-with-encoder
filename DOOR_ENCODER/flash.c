/*
 * flash.c
 *
 *  Created on: 03/02/2015
 *      Author: Mario
 */

#include "flash.h"
#include "definitions.h"

unsigned int current_ptr_offset(){

	unsigned int i;
	char *Flash_ptrD;
	Flash_ptrD = FLASH_SEG_D;             // Initialize Flash segment D ptr

	for (i = 0; i < 256; i++)            // Leer los dos primeros segmentos
	{
		if( Flash_ptrD[i] == 0xFF)          // copy value segment C to seg D
			return i;
	}
	return 0;

}
void erase_flash(char *Flash_ptr){

    FCTL3 = FWKEY; // Clear LOCK
	FCTL1 = FWKEY+ERASE; //Enable segment erase
	*Flash_ptr = 0; // Dummy write, erase Segment
	while(FCTL3 & BUSY);
	FCTL3 = FWKEY+LOCK; // Done, set LOCK

}

void write_flash_c(char value, unsigned int offset){

  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = FLASH_SEG_D;              	// Segment D
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation
  Flash_ptr[offset] = value;
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}

void write_flash_i(int value, unsigned int offset)
{
  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = FLASH_SEG_D;              // Segment D
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  Flash_ptr[offset]   = (value & 0x00FF);
  Flash_ptr[offset+1] = (value & 0xFF00)>>8;

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}

