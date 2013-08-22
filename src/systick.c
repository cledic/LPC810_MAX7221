#include "systick.h"                        /* LPC8xx definitions */

#define SYSTICK_DELAY		(SystemCoreClock/100)

volatile uint32_t TimeTick = 0;

/* SysTick interrupt happens every 10 ms */
void SysTick_Handler(void)
{
  TimeTick++;
}

void SysTick_Init( void)
{

  TimeTick=0;
  /* Called for system library in core_cmx.h(x=0 or 3). */
  SysTick_Config( SYSTICK_DELAY );

}

void SysTick_WakeUp( void)
{

  TimeTick=0;
  /* Called for system library in core_cmx.h(x=0 or 3). */
  SysTick_Config( 16000000 );

}

void Systick_Stop( void)
{
	SysTick->CTRL = 0;
}

uint32_t getTimerTick( void)
{
	return TimeTick;
}

uint32_t getTimerTickDiff( uint32_t t)
{
	return (TimeTick - t);
}

