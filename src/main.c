/**************************************************************************/
/*!
    @file     main.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include <stdio.h>
#include <string.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "power_mode.h"

#include "max7221_lib.h"
#include "max7221_graph.h"

/* */
void SwitchMatrix_Init( void);
unsigned char MsgParser( void);
char Hex2Num( char c);

/* */
#define OUTPUT		1
#define INPUT		0

/* */
#define FALSE	0
#define TRUE	1

#if defined(__CODE_RED)
  #include <cr_section_macros.h>
  #include <NXP/crp.h>
  __CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#endif

extern char line[];

char text[cMAX_STR_LEN];
unsigned long tmr;
volatile unsigned char msg_done;
unsigned char matrixColor=0;
unsigned char inactivity=0;

int main(void)
{

  /* Configure the core clock/PLL via CMSIS */
  SystemCoreClockUpdate();

  /* Initialise the GPIO block */
  gpioInit();

  /* Initialise the UART block */
  uart0Init( 230400);

  /*Configure the Systick timer for 100Hz period */
  SysTick_Init();

  /* Configure the SPI0 interface */
  spiInit( LPC_SPI0, 5, SPI_DLY_POSTDELAY(2)|SPI_DLY_PREDELAY(2));				// div=5 -> 2Mhz

  SwitchMatrix_Init();

  // Starting msg...
  text[0] = 'H';
  text[1] = 'e';
  text[2] = 'l';
  text[3] = 'l';
  text[4] = 'o';
  text[5] = '!';
  text[6] = '\0';

  while(1) {
	  //
	  switch (matrixColor) {
	  case 0:
		  // In this case is scrolling initial msg...
		  MAX7221_ScrollText( &text[0], cMATRIX_GREEN);
		  uart0SendChar('!');				// Msg Done
		  break;
	  case 1:
	  case 2:
		  // Scroll msg...
		  MAX7221_ScrollText( &text[0], matrixColor);
		  uart0SendChar('!');				// Msg Done
		  break;
	  case 3:
		  // Predefined figure...
		  MAX7221_Figure( text[0], text[1]);
		  uart0SendChar('!');				// Msg Done
		  break;
	  case 4:
		  // Custom figure...
		  MAX7221_CustomFigure( &text[0], text[8]);
		  uart0SendChar('!');				// Msg Done
		  break;
	  case 5:
		  // Shutdown matrix and MCU...
		  inactivity = 99;
		  matrixColor=0;			// Force a deep power down
		  break;
	  }

	  tmr=getTimerTick();
	  while( getTimerTickDiff( tmr) < 50) {		// .5 sec delay
		  if ( msg_done)						// break here if msg coming...
			  break;
	  }

	  if ( msg_done) {
		  msg_done=0;
		  inactivity=0;
		  //
		  if ( MsgParser()) {
			  uart0SendChar('.');			// OK
		  } else {
			  uart0SendChar('?');			// Bad
		  }
	  } else {
		  inactivity++;
		  if ( inactivity >= 12) {
			  //
			  Systick_Stop();
			  power_down();
		  }
	  }
  }

}

/** Parse the messages received from serial port.
 *
 * @param	nothing
 * @return	0 in case of ERROR, 1 for a msg to show
 *
*/
unsigned char MsgParser( void)
{
  unsigned char i = 0;
  unsigned char tmp = 0;

  while (line[i] != '\0')
  {
    if (line[i] == ',')  	// separator found
    {
      line[i] = '\0';		// close the payload field
      i++;
      tmp = line[i] - '0';	// read the matrix color field
      break;
    }
    i++;
    // check payload lenght
    if ( i >= 32)
    	return 0;				// nothing done...
  }

  // Fields structure
  //  0           1
  // <payload[32],m>
  //

  // Check color matrix value
  if ( tmp == 0 || tmp > 5)
	  return 0;				// nothing done...

  // Message inside the payload...
  if ( tmp==1 || tmp==2) {
	  i=0;
	  while( line[i] != '\0') {
		  text[i] = line[i];	// copy the payload message...
		  i++;
	  }
	  //
	  text[i] = '\0';
	  matrixColor = tmp;		// copy the matrix color to use...
	  //
	  return 1;
  }
  // Embedded figure
  if ( tmp == 3) {
	  text[0] = line[0] -'0';	// figure number
	  text[1] = line[1] -'0';	// color: reg or green
	  matrixColor = tmp;
	  return 1;
  }
  // Custom figure
  if ( tmp == 4) {
	  text[0] = (Hex2Num( line[0] )<<4) | Hex2Num( line[1] );
	  text[1] = (Hex2Num( line[2] )<<4) | Hex2Num( line[3] );
	  text[2] = (Hex2Num( line[4] )<<4) | Hex2Num( line[5] );
	  text[3] = (Hex2Num( line[6] )<<4) | Hex2Num( line[7] );
	  text[4] = (Hex2Num( line[8] )<<4) | Hex2Num( line[9] );
	  text[5] = (Hex2Num( line[10] )<<4) | Hex2Num( line[11] );
	  text[6] = (Hex2Num( line[12] )<<4) | Hex2Num( line[13] );
	  text[7] = (Hex2Num( line[14] )<<4) | Hex2Num( line[15] );
	  //
	  text[8] = line[16] - '0';	// color: red or green
	  matrixColor = 4;
	  //
	  return 1;
  }
  // Sleep...
  if ( tmp == 5) {
	  matrixColor = 5;
	  return 1;
  }
  //
  return 0;
}

/** Ipotesi di utilizzo payload e carattere m
* m -> 	un carattere ASCII che può assumere i seguenti valori:
*		1: in payload è presente una stringa, max 32 char, da visualizzare in ROSSO
*		2: come opz. 1 ma da visualizzare in VERDE
*		3: il primo carattere del payload è l'indice, da 0 a F, alla figura embedded da visualizzare
*		   il secondo carattere indica il colore: red o green.
*		4: per inviare una figura custom alla matrice. Il payload è di 17 byte. I primi 16 sono la versione
*		   ASCII degli 8byte da visualizzare, a seguire il colore: red o green.
*		5: mette il dispositivo in sleep
* payload[32]	->	stringa da visualizzare o altro a seconda del parametro m.
*
*/

/** Convert an ASCII hex character to numerical value.
 *
 *  @param	c	ASCII character to convert.
 *  @return		return the numerical value: 0->15
 */
char Hex2Num( char c)
{
	if ( c >= '0' && c <= '9')
		return c-'0';
	if ( c >= 'A' && c <= 'F')	// Upper case A -> 65 (ASCII)
		return c-55;			// ...to return 10
	if ( c >= 'a' && c <= 'f')	// Lower case a -> 97 (ASCII)
		return c-87;			// ...to return 10

	return 0;
}


/*
 * Pins configuration to drive two MAX7221:
 *
 * pin 1 -> RESET		pin 8 -> RX (ISP)
 * pin 2 -> TX (ISP)	pin 7 -> Vss
 * pin 3 -> MOSI		pin 6 -> Vdd
 * pin 4 -> CS			pin 5 -> SCK
 */
void SwitchMatrix_Init()
{
    /* Enable SWM clock */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

    /* Pin Assign 8 bit Configuration */
    /* U0_TXD */
    /* U0_RXD */
    LPC_SWM->PINASSIGN0 = 0xffff0004UL;
    /* SPI0_SCK */
    LPC_SWM->PINASSIGN3 = 0x01ffffffUL;
    /* SPI0_MOSI */
    /* SPI0_SSEL */
    LPC_SWM->PINASSIGN4 = 0xff02ff03UL;

    /* Pin Assign 1 bit Configuration */
    /* RESET */
    LPC_SWM->PINENABLE0 = 0xffffffbfUL;
}

