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
uint8_t door_menu(void);
uint8_t door_set_limits(uint8_t);
void door_move(void);

uint8_t State, LastState;
uint8_t StateEnc, LastStateEnc;
uint8_t StateMenu, bMenu = 0;

uint16_t ciclos, timer_save=0;
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
  erase_flash(FLASH_SEG_D);
  __no_operation();
  write_flash_c(0xBA,0);
  __no_operation();
  write_flash_i(1982,1);
  __no_operation();

  config_interrupts();
  lcdclear(2);

  State = STATE_INIT;

    while (1) {

		if( I_BUTTON_LEFT_PRESSED ){
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
	  P1OUT  = 0x00;

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
	  TA1CCR0  = 50000;
	  TA1CTL   = TASSEL_2 + MC_2 + TACLR;         // SMCLK, contmode, clear TAR

	  P1IES |=  BIT1;                            // P1.4 Hi/Lo edge
	  P1IFG &= ~BIT1;                           // P1.4 IFG cleared
	  P1IE  |=  BIT1;                             // P1.4 interrupt enabled

	  __enable_interrupt();
}

// Timer1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  P1OUT   ^= LED0;                            // Toggle P1.0
  TA1CCR0 += 50000;                         // Add Offset to CCR0

  // Finite state machine:
	switch(State)
	{
		/**************************************************/
		case STATE_INIT:

			lcdclear(1);
			prints("En Operacion");
			gotoXy(0,1); integerToLcd(ciclos);

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF

			//if()
			door_set_limits(REASON_NO_LIMITS);

			//Si mi ultimo estado es que estaba subiendo o bajando, se ha perdido
			//la referencia. Hay que configurar límites de nuevo
			door_set_limits(REASON_DOOR_LOST);

			//TENGO QUE LEER MI ULTIMO ESTADO PARA SABER DONDE ESTOY

		break;
		/**************************************************/
		case STATE_DOOR_IS_DOWN:

			lcdclear(2);
			prints("En Operacion");
			gotoXy(0,1); integerToLcd(ciclos);

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF

			if( P1IN & I_BUTTON )
			{
				State = STATE_OPENING;
			}

		break;
		/**************************************************/
		case STATE_OPENING:

			lcdclear(1);
			prints("Abriendo Puerta..");

			P1OUT &= ~(O_DOOR_UP);     // 0 - ON
			P1OUT |= O_DOOR_DOWN;      // 1 - OFF


		break;
		/**************************************************/
		case STATE_DOOR_IS_UP:

			lcdclear(2);
			prints("Puerta Abierta");
			gotoXy(0,1); integerToLcd(ciclos);

			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF
			//leer del pot
		break;

		/**************************************************/
		case STATE_CLOSING:

			lcdclear(1);
			prints("Cerrando puerta..");

			P1OUT &= ~(O_DOOR_DOWN); 	// 0 - ON
			P1OUT |= O_DOOR_UP;   // 1 - OFF

		break;
		/**************************************************/
		case STATE_MENU:

			TA1CCTL0 &= ~CCIE;
			P1OUT |= (O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF
			bMenu = door_menu();
			TA1CCTL0 |=  CCIE;

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

uint8_t door_menu(void){
// Menu de la puerta:
// 1. Ver límites
// 2. Configurar límites

	return 0;
}

uint8_t door_set_limits(uint8_t reason){

	lcdclear(2);

	switch (reason){
		case REASON_NO_LIMITS:
			prints("No hay limites."); gotoXy(0,1); prints("OK");
			while(!(I_BUTTON_LEFT_PRESSED));
		break;
		case REASON_DOOR_LOST:
			prints("Redefinir ref."); gotoXy(0,1); prints("OK");
			while(!(I_BUTTON_LEFT_PRESSED));
		break;
		case REASON_MANUAL:
			prints("Cambiar limites?");
			prints("SI            NO");
			do{
				if(I_BUTTON_RIGHT_PRESSED)
				{
					while(I_BUTTON_RIGHT_PRESSED);
					return 0;
				}
			}while(!(I_BUTTON_LEFT_PRESSED));
		break;
	}
	while(I_BUTTON_LEFT_PRESSED);

	lcdclear(2);
	prints("Limite Cerrar");
	gotoXy(0,1); prints("SUBIR     BAJAR");
	door_move();
	//guardar valor leido por encoder
	lcdclear(1);
	prints("Limite Abrir");
	door_move();
    //guardar valor leido por encoder
	return 1;

}

void door_move(){

	do{
		if(I_BUTTON_LEFT_PRESSED){
			timer_save = 0;
			P1OUT &= ~(O_DOOR_UP);     // 0 - ON
			P1OUT |= O_DOOR_DOWN;      // 1 - OFF
			while(I_BUTTON_LEFT_PRESSED);
			P1OUT &= ~(O_DOOR_UP + O_DOOR_DOWN);     // 0 - ON
		}
		else if(I_BUTTON_RIGHT_PRESSED){
			timer_save = 0;
			P1OUT &= ~(O_DOOR_DOWN);     // 0 - ON
			P1OUT |= O_DOOR_UP;      // 1 - OFF
			while(I_BUTTON_RIGHT_PRESSED);
			P1OUT &= ~(O_DOOR_UP + O_DOOR_DOWN);     // 0 - ON
		}
	}while(timer_save < 1000);
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  ciclos++;;                            // P1.0 = toggle
  gotoXy(0,1);
  integerToLcd(ciclos);
  P1IFG &= ~BIT1;                          // P1.4 IFG cleared

  switch(__even_in_range(P1IV,16))
  	{
		case   0:  break;  //  No  Interrupt
		case   2:  break;  //  P1.0
		case   4:  break;  //  P1.1
		case   6:  break;  //  P1.2
		case   8:  break;  //  P1.3
		case   10: break;  //  P1.4
		case   12: break;  //  P1.5
		case   14: break;  //  P1.6
		case   16: break;  //  P1.7
  	}


}


