//***************************************************************************************
//
//  Medarz Ingenieria SA de CV
//
//***************************************************************************************

#include <msp430.h>
#include <stdint.h>
#include "definitions.h"
#include "lcd.h"
#include "flash.h"

void config_ports(void);
void config_interrupts(void);

unsigned int State, LastState;
uint16_t ciclos;
uint16_t upper_limit;
uint16_t lower_limit;

int main(void) {

  ciclos = 0;
  WDTCTL = WDTPW + WDTHOLD;

  config_ports();
  lcdinit();

  prints("** MEDARZ ING **");
  __delay_cycles(2000000);
  gotoXy(0,1);
  prints("Inicializando...");
  __delay_cycles(3000000);
  __no_operation();
  erase_flash();
  __no_operation();
  write_flash_c(0xBA,0);
  __no_operation();
  write_flash_i(1982,1);
  __no_operation();

  config_interrupts();

  lcdclear();

  State = STATE_INIT;

    while (1) {

		if((P2IN & BIT1)==0){
			P1OUT |= BIT0;
			while((P2IN & BIT1)==0);
   		    ciclos--;
   		    gotoXy(0,1);
   		    integerToLcd(ciclos);
			P1OUT &= ~BIT0;
		}
	}
	return 0;
}

void config_ports(){


	/************************************************************/
	/*                        OUTPUTs                           */
	/************************************************************/
	  P1DIR |= LED0;
	  P1OUT = 0x00;

	/************************************************************/
	/*                        INPUTs                            */
	/************************************************************/
	  P1DIR &= ~ (I_BUTTON_RIGHT);
	  P1REN |= I_BUTTON_RIGHT;                            // Enable internal resistance
	  P1OUT |= I_BUTTON_RIGHT;                            // Set as pull-Up resistance

	  P2DIR &= ~ (I_BUTTON_LEFT);
	  P2REN |= I_BUTTON_LEFT;                            // Enable internal resistance
	  P2OUT |= I_BUTTON_LEFT;                            // Set as pull-Up resistance

}

void config_interrupts(){

	  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
	  TA1CCR0 = 50000;
	  TA1CTL = TASSEL_2 + MC_2 + TACLR;         // SMCLK, contmode, clear TAR

	  P1IES |= BIT1;                            // P1.4 Hi/Lo edge
	  P1IFG &= ~BIT1;                           // P1.4 IFG cleared
	  P1IE |= BIT1;                             // P1.4 interrupt enabled

	  __enable_interrupt();
}

// Timer1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  P1OUT ^= LED0;                            // Toggle P1.0
  TA1CCR0 += 50000;                         // Add Offset to CCR0

  // Finite state machine:
	switch(State)
	{
		/**************************************************/
		case STATE_INIT:
			
			gotoXy(0,0); prints("En Operacion");
			gotoXy(0,1); integerToLcd(ciclos);
			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF

			//TENGO QUE LEER MI ULTIMO ESTADO PARA SABER DONDE ESTOY



		break;
		/**************************************************/
		case STATE_DOOR_IS_DOWN:
			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF

			if( P1IN & I_BUTTON )
			{
				State = STATE_OPENING;
			}
		break;
		/**************************************************/
		case STATE_OPENING:

			P1OUT &= ~(O_DOOR_UP);     // 0 - ON
			P1OUT |= O_DOOR_DOWN;      // 1 - OFF


		break;
		/**************************************************/
		case STATE_DOOR_IS_UP:

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF


			if( ((P1IN & I_SAFETY_SENSOR)==0) || P1IN & I_BUTTON )
			{

			}


			//leer del pot
		break;

		/**************************************************/
		case STATE_CLOSING:

			P1OUT &= ~(O_DOOR_DOWN); 	// 0 - ON
			P1OUT |= O_DOOR_UP;   // 1 - OFF
			if( ((P1IN & I_SAFETY_SENSOR)==0) || P1IN & I_BUTTON )
			{
				State = STATE_WAIT_AND_UP;
			}


		break;
		/**************************************************/
		case STATE_WAIT_AND_UP:

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF
			_delay_cycles(1000000);
			State = STATE_OPENING;

		break;
		/**************************************************/
		case STATE_ESTOP:

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF
			if( P1IN & I_EMERGENCY_STOP)
			{
				State = LastState;
			}

		break;

	}



}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  ciclos++;;                            // P1.0 = toggle
  gotoXy(0,1);
  integerToLcd(ciclos);
  P1IFG &= ~BIT1;                          // P1.4 IFG cleared
}


