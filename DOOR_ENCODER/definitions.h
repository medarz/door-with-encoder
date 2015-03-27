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
 * PORTS
 *----------------------------------------------------------------------------*/
#define		PORT_DOOR_RELAYS		P3OUT
#define		PORT_DOOR_RELAYS_DIR	P3DIR

#define		PORT_LED0				P1OUT
#define		PORT_LED0_DIR			P1DIR

#define		PORT_LED1				P4OUT
#define		PORT_LED1_DIR			P4DIR


/*------------------------------------------------------------------------------
 * INPUTS
 *----------------------------------------------------------------------------*/
#define 	I_BUTTON_RIGHT			BIT1
#define		I_BUTTON_RIGHT_PRESSED  (P1IN & BIT1) == 0

#define 	I_BUTTON_LEFT			BIT1
#define		I_BUTTON_LEFT_PRESSED   (P2IN & BIT1) == 0

#define 	I_BUTTON_CENTER			BIT6
#define		I_BUTTON_CENTER_PRESSED (P2IN & BIT6) == 0

#define 	I_BUTTON				BIT0
#define		I_BUTTON_PRESSED   		(P2IN & BIT0) == 0

#define		I_EMERGENCY_STOP		BIT4

#define 	I_CHAN1					BIT2
#define		I_CHAN2					BIT3
/*------------------------------------------------------------------------------
 * OUTPUTS
 *----------------------------------------------------------------------------*/
#define		O_DOOR_DOWN				BIT0
#define		O_DOOR_UP				BIT1

#define     LED0					BIT0
#define 	LED0_ON					PORT_LED0 |= LED0
#define     LED0_OFF				PORT_LED0 &= ~LED0

#define     LED1					BIT7
#define 	LED1_ON					PORT_LED1 |= LED1
#define     LED1_OFF				PORT_LED1 &= ~LED1

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
#define  STATE_LIMIT_DEFINITION		0x80

/*------------------------------------------------------------------------------
 * STATE MACHINE ENCODER
 *----------------------------------------------------------------------------*/
#define  STATE_S1	 		    	0x03
#define  STATE_S2					0x02
#define  STATE_S3			  		0x01

/*------------------------------------------------------------------------------
 * 	TIMERS
 *----------------------------------------------------------------------------*/
#define  WAIT_FOR_MENU		  		1000
#define  WAIT_FOR_LIMIT_TIMEOUT     180

/*------------------------------------------------------------------------------
 * 	MENU RELATED
 *----------------------------------------------------------------------------*/
#define  REASON_NO_LIMITS	  		0x01
#define  REASON_MANUAL				0x02
#define	 REASON_DOOR_LOST			0x03

/*------------------------------------------------------------------------------
 * 	FLASH SEGMENTS
 *----------------------------------------------------------------------------*/

#define	 FLASH_SEG_D				(char *) 0x1800
#define	 FLASH_SEG_C				(char *) 0x1880
#define	 FLASH_SEG_B				(int *) 0x1900
#define	 FLASH_SEG_A				(int *) 0x1980


#endif /* DEFINITIONS_H_ */
