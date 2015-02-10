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
void door_init(void);
void door_move(void);
void door_lcd(char *);
void door_save_state(void);

uint8_t State=0, NextState=0;
uint8_t StateEnc, LastStateEnc;
uint8_t StateMenu, bMenu = 0;
uint8_t bNoMove = 0, bLCDPrint=0, bSaveState=0, config_limits = 0;
uint8_t offset_state, offset_ciclos;

uint16_t ciclos, timer_save=0;
uint16_t upper_limit;
uint16_t lower_limit;
uint16_t encoder_position=0;


int main(void) {

  ciclos = 0;
  WDTCTL = WDTPW + WDTHOLD;

  config_ports();
  lcdinit();

  prints("** MEDARZ ING **");
  __delay_cycles(2000000);
  gotoXy(0,1);
  prints("Inicializando...");
  __delay_cycles(1000000);

  config_interrupts();
  lcdclear(2);
  door_init();
  //erase_flash(FLASH_SEG_D);
  NextState = STATE_INIT;
  __enable_interrupt();

  while (1) {

		if(config_limits == REASON_NO_LIMITS){
			config_limits = door_set_limits(REASON_NO_LIMITS);
		}
		else if(config_limits == REASON_DOOR_LOST)
		{
			config_limits = door_set_limits(REASON_DOOR_LOST);
		}
		else if(config_limits == REASON_MANUAL)
		{
			door_set_limits(REASON_MANUAL);
			config_limits = 0;
		}
	   //Hay una transicion de estado.

	   if(bSaveState && State != STATE_INIT && NextState == State)
	   {
		  bSaveState = 0;
		  write_flash_c(State, offset_state);
		  offset_state++;
	   }

	}
	return 0;
}

void config_ports(){


	/************************************************************/
	/*                        OUTPUTs                           */
	/************************************************************/
	  PORT_LED0_DIR |=  LED0;
	  PORT_LED0     &= ~LED0;

	  PORT_LED1_DIR |=  LED1;
	  PORT_LED1     &= ~LED1;

	  PORT_DOOR_RELAYS_DIR |=  O_DOOR_DOWN + O_DOOR_UP;
	  PORT_DOOR_RELAYS     &= ~(O_DOOR_DOWN + O_DOOR_UP); //OFF

	/************************************************************/
	/*                        INPUTs                            */
	/************************************************************/
	  P1DIR &= ~ (I_BUTTON_RIGHT + I_CHAN1 + I_CHAN2);
	  P1REN |= I_BUTTON_RIGHT + I_CHAN1 + I_CHAN2;                            // Enable internal resistance
	  P1OUT |= I_BUTTON_RIGHT + I_CHAN1 + I_CHAN2;                            // Set as pull-Up resistance

	  P2DIR &= ~ (I_BUTTON_LEFT);
	  P2REN |= I_BUTTON_LEFT;                            // Enable internal resistance
	  P2OUT |= I_BUTTON_LEFT;                            // Set as pull-Up resistance

}

void config_interrupts(){

	  TA1CCTL0 |= CCIE;                          // CCR0 interrupt enabled
	  TA1CCR0  = 50000;
	  TA1CTL   = TASSEL_2 + MC_1 + TACLR;         // SMCLK, contmode, clear TAR

	  //TBCTL = TBSSEL_1 + MC_1 + ID_3;         // SMCLK, contmode, clear TAR

	  P1IES |=   I_CHAN1 + I_CHAN2;                             // Hi -> Lo edge
	  P1IFG &= ~(I_CHAN1 + I_CHAN2);                           // IFG cleared
	  P1IE  |=   I_CHAN1 + I_CHAN2;                             // Interrupt enabled

}

// Timer1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  //P1OUT   ^= LED0;                            // Toggle P1.0
  char * statePtr = FLASH_SEG_D;


	if(NextState != State )
	{
	  if(offset_state>=256) offset_state = 0;
	  bLCDPrint = 1; bSaveState = 1;
	  State = NextState;
	}
  // Finite state machine:
	switch(State)
	{
		/**************************************************/
		case STATE_INIT:

			door_lcd("En Operacion");
			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN); // 1 - RELAYS OFF

			offset_state = current_ptr_offset(1);

			if(offset_state>0)
			{
				//TENGO QUE LEER MI ULTIMO ESTADO PARA SABER DONDE ESTOY
				if    ( statePtr[offset_state - 1 ] == STATE_DOOR_IS_DOWN) 		 NextState = STATE_DOOR_IS_DOWN;
				else if(statePtr[offset_state - 1 ] == STATE_DOOR_IS_UP )  		 NextState = STATE_DOOR_IS_UP;
				else config_limits = REASON_DOOR_LOST;
			}
			else
				config_limits = REASON_DOOR_LOST;

		break;
		/**************************************************/
		case STATE_DOOR_IS_DOWN:

			door_lcd("Puerta Cerrada");

			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);

			/*if( P1IN & I_BUTTON )
			{
				NextState = STATE_OPENING;
			}*/

		break;
		/**************************************************/
		case STATE_OPENING:

			door_lcd("Abriendo Puerta");

			PORT_DOOR_RELAYS |= (O_DOOR_UP);
			PORT_DOOR_RELAYS &= ~O_DOOR_DOWN;


		break;
		/**************************************************/
		case STATE_DOOR_IS_UP:

			door_lcd("Puerta Abierta");

			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);
			//leer del pot
		break;

		/**************************************************/
		case STATE_CLOSING:

			door_lcd("Cerrando puerta..");

			PORT_DOOR_RELAYS |= (O_DOOR_DOWN);
			PORT_DOOR_RELAYS &= ~O_DOOR_UP;

		break;
		/**************************************************/
		case STATE_MENU:
			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);
			bMenu = door_menu();
		break;
		/**************************************************/
		case STATE_LIMIT_DEFINITION:
			 timer_save++;
		break;
		/**************************************************/
		case STATE_ESTOP:

			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);
			if( P1IN & I_EMERGENCY_STOP)
			{

			}

		break;
	}
}

void save_door_state(void){

}

void  door_lcd(char *message)
{
	if( bLCDPrint ){
		lcdclear(2);
		prints(message);
		gotoXy(0,1); integerToLcd(ciclos);
		bLCDPrint=0;
	}

}

void door_init(){

	int  *limites    = FLASH_SEG_A;
	int  *tot_ciclos = FLASH_SEG_B;

	//Obtener los ciclos que se tienen actualmente
	offset_ciclos = current_ptr_offset(2);
	if(offset_ciclos == 0)
		ciclos = 0;
	else
		ciclos = tot_ciclos[offset_ciclos-1];

	//Vemos si ya hay limites guardados
    if(limites[0] == 0xFFFF && limites[1] == 0xFFFF)
    {
    	config_limits = REASON_NO_LIMITS;
    }
    else
    {
    	upper_limit = limites[1];
    	lower_limit = limites[0];
    }
}

uint8_t door_menu(void){
// Menu de la puerta:
// 1. Ver limites
// 2. Configurar limites

	return 0;
}

uint8_t door_set_limits(uint8_t reason){

	lcdclear(2);

	NextState = STATE_LIMIT_DEFINITION;

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

	timer_save=0;

	lcdclear(2);
	prints("Limite Abrir");
	gotoXy(0,1); prints("SUBIR      BAJAR");
	door_move();
	//guardar valor leido por encoder
	if(bNoMove)
	{
		bNoMove = 0; timer_save = 0;
		lcdclear(1);
		prints("Limite Cerrar");
		door_move();
		if(!bNoMove) { return reason; }
	}
	else
	{
		return reason;
	}

    //guardar valor leido por encoder

	lcdclear(2);
	prints("Lim. Modificados");
	__delay_cycles(2000000);
	NextState = STATE_DOOR_IS_DOWN;
	return 0;

}

void door_move(){

	do{
		if(I_BUTTON_LEFT_PRESSED){
			timer_save = 0; bNoMove=1;
			PORT_DOOR_RELAYS &= ~(O_DOOR_UP);     // 0 - Off
			PORT_DOOR_RELAYS |= O_DOOR_DOWN;      // 1 - On
			LED0_ON;
			while(I_BUTTON_LEFT_PRESSED);
			LED0_OFF;
			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);     // 0 - OFF
		}
		else if(I_BUTTON_RIGHT_PRESSED){
			timer_save = 0; bNoMove=1;
			PORT_DOOR_RELAYS &= ~(O_DOOR_DOWN);     // 0 - Off
			PORT_DOOR_RELAYS |= O_DOOR_UP;      // 1 - On
			LED1_ON;
			while(I_BUTTON_RIGHT_PRESSED);
			LED1_OFF;
			PORT_DOOR_RELAYS &= ~(O_DOOR_UP + O_DOOR_DOWN);     // 0 - ON
		}
	}while( timer_save < WAIT_FOR_LIMIT_TIMEOUT );
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  gotoXy(0,1);
  integerToLcd(ciclos);
  P1IFG &= ~BIT1;                          // P1.4 IFG cleared

  //Aqui haremos la maquina de estados del encoder.
  encoder_position++;

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

#pragma vector=TIMERB1_VECTOR
__interrupt void TIMER1_ISR(void)
{
	 switch( __even_in_range(TBIV,14) )
	  {
	    case  0: break;                          // No interrupt
	    case  2: break;                          // CCR1 not used
	    case  4: break;                          // CCR2 not used
	    case  6: break;                          // CCR3 not used
	    case  8: break;                          // CCR4 not used
	    case 10: break;                          // CCR5 not used
	    case 12: break;                          // CCR6 not used
	    case 14: break;							//overflow
	    default: break;
	  }
}


