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
#define 	I_BUTTON_RIGHT			BIT1
#define		I_BUTTON_RIGHT_PRESSED  (P1IN & BIT1) == 0

// ---- PORT 2 ----
#define 	I_BUTTON_LEFT			BIT1
#define		I_BUTTON_LEFT_PRESSED   (P2IN & BIT1) == 0

#define		I_BUTTON				BIT3
#define		I_EMERGENCY_STOP		BIT4

/*------------------------------------------------------------------------------
 * OUTPUTS
 *----------------------------------------------------------------------------*/
#define		O_DOOR_DOWN				BIT6
#define		O_DOOR_UP				BIT7
#define     LED0					BIT0

/*------------------------------------------------------------------------------
 * STATE MACHINE DOOR
 *----------------------------------------------------------------------------*/
#define  STATE_INIT 		    	0x10
#define  STATE_DOOR_IS_DOWN			0x20
#define  STATE_DOOR_IS_UP 	  		0x30
#define  STATE_OPENING		  		0x40
#define  STATE_CLOSING				0x50
#define  STATE_ESTOP				0x60
#define  STATE_DOOR_IS_LOST			0x70
#define  STATE_MENU					0xEF

/*------------------------------------------------------------------------------
 * STATE MACHINE ENCODER
 *----------------------------------------------------------------------------*/
#define  STATE_STILL 		    	0x11
#define  STATE_MOVE_LEFT			0x10
#define  STATE_MOVE_RIGHT 	  		0x01
#define  STATE_INCREASE		  		0x00

/*------------------------------------------------------------------------------
 * 	TIMERS
 *----------------------------------------------------------------------------*/
#define  WAIT_FOR_MENU		  		1000
#define  WAIT_FOR_LIMIT_TIMEOUT     1000

/*------------------------------------------------------------------------------
 * 	MENU RELATED
 *----------------------------------------------------------------------------*/
#define  REASON_NO_LIMITS	  		0x00
#define  REASON_MANUAL				0x01
#define	 REASON_DOOR_LOST			0x02

/*------------------------------------------------------------------------------
 * 	FLASH SEGMENTS
 *----------------------------------------------------------------------------*/

#define	 FLASH_SEG_D				(char *) 0x1800
#define	 FLASH_SEG_C				(char *) 0x1880
#define	 FLASH_SEG_B				(char *) 0x1900

#endif /* DEFINITIONS_H_ */
