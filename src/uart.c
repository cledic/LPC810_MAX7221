/**************************************************************************/
/*!
    @file     uart.c
    @author   K. Townsend

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
#include <string.h>

#include "uart.h"
#include "systick.h"

/* Buffer for character received */
char line[ cBUFFER];
/* External flag for set msg available state */
extern unsigned char msg_done;
/* Buffer index */
static unsigned char idx=0;

void uart0Init(uint32_t baudRate)
{
  uint32_t clk;

  /* Setup the clock and reset UART0 */
  LPC_SYSCON->UARTCLKDIV = 1;
  NVIC_DisableIRQ(UART0_IRQn);
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 14);
  LPC_SYSCON->PRESETCTRL    &= ~(1 << 3);
  LPC_SYSCON->PRESETCTRL    |=  (1 << 3);

  /* Configure UART0 */
  clk = SystemCoreClock/LPC_SYSCON->UARTCLKDIV;
  LPC_USART0->CFG = UART_DATA_LENGTH_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
  LPC_USART0->BRG = clk / 16 / baudRate - 1;
  LPC_SYSCON->UARTFRGDIV = 0xFF;
  LPC_SYSCON->UARTFRGMULT = (((clk / 16) * (LPC_SYSCON->UARTFRGDIV + 1)) /
    (baudRate * (LPC_USART0->BRG + 1))) - (LPC_SYSCON->UARTFRGDIV + 1);

  /* Clear the status bits */
  LPC_USART0->STAT = UART_STATUS_CTSDEL | UART_STATUS_RXBRKDEL;

  /* Enable UART0 interrupt */
  NVIC_EnableIRQ(UART0_IRQn);

  LPC_USART0->INTENSET = (0x01<<0);		// Enable RX IRQ
  LPC_USART0->INTENCLR = (0x01<<2);		// Disable TX IRQ

  /* Enable UART0 */
  LPC_USART0->CFG |= UART_ENABLE;
}

void UART0_IRQHandler(void)
{
  uint32_t Status = 0, regVal;

  Status = LPC_USART0->STAT;
  if (Status & (0x01<<0))	/* RX Ready */
  {
	 if ( idx==0)
		 msg_done=0;

	/* Receive Data Available */
	regVal=LPC_USART0->RXDATA;
    switch( regVal)
    {
    case '\r':
    case '\n':  // ignore linebreaks
      break;
    case '<' :  // new buffer with messages starts
      idx = 0;
      break;
    case '>':   // line ready
      msg_done = 1;
      idx = 0;
      break;
    default:
    	line[idx++] = regVal & 0xFF;
    	if ( idx >= cBUFFER)
    		idx=0;
    	line[idx] = '\0';
    }
  }
  return;
}

void uart0SendChar(char buffer)
{
  /* Wait until we're ready to send */
  while (!(LPC_USART0->STAT & UART_STATUS_TXRDY));
  LPC_USART0->TXDATA = buffer;
}

void uart0Send(char *buffer, uint32_t length)
{
  while (length != 0)
  {
    uart0SendChar(*buffer);
    buffer++;
    length--;
  }
}

char uart0ReceiveChar( void)
{
  /* Wait until there is a char available */
  while (!(LPC_USART0->STAT & UART_STATUS_RXRDY));
  return LPC_USART0->RXDATA;
}

uint32_t uart0ReceiveReady( void)
{
	if ( LPC_USART0->STAT & UART_STATUS_RXRDY)
		return 1;
	else
		return 0;
}

