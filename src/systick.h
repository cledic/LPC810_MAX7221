#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "LPC8xx.h"

void SysTick_Init( void);
//
uint32_t getTimerTick( void);
void Systick_Stop( void);
void SysTick_WakeUp( void);
uint32_t getTimerTickDiff( uint32_t t);

#endif
