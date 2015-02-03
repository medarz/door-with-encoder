/*
 * definitions.h
 *
 *  Created on: 05/11/2015
 *      Author: Mario David Medina Arzate
 *      MEDARZ INGENIERIA
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/*------------------------------------------------------------------------------
 * INPUTS
 *----------------------------------------------------------------------------*/

// ---- PORT 1 ----
#define 	I_BUTTON_RIGHT		BIT1

// ---- PORT 2 ----
#define 	I_BUTTON_LEFT		BIT1

#define		I_SAFETY_SENSOR		BIT2
#define		I_BUTTON			BIT3
#define		I_EMERGENCY_STOP	BIT4

/*------------------------------------------------------------------------------
 * OUTPUTS
 *----------------------------------------------------------------------------*/
#define		O_DOOR_DOWN			BIT6
#define		O_DOOR_UP			BIT7
#define     LED0				BIT0


/*------------------------------------------------------------------------------
 * STATE MACHINE DOOR
 *----------------------------------------------------------------------------*/
#define  STATE_INIT 		    	0x10
#define  STATE_DOOR_IS_DOWN			0x20
#define  STATE_DOOR_IS_UP 	  		0x30
#define  STATE_OPENING		  		0x40
#define  STATE_CLOSING				0x50
#define  STATE_ESTOP				0x60
#define  STATE_WAIT_AND_UP			0x70

/*------------------------------------------------------------------------------
 * STATE MACHINE ENCODER
 *----------------------------------------------------------------------------*/
#define  STATE_INIT 		    	0x10
#define  STATE_DOOR_IS_DOWN			0x20
#define  STATE_DOOR_IS_UP 	  		0x30
#define  STATE_OPENING		  		0x40


#endif /* DEFINITIONS_H_ */
