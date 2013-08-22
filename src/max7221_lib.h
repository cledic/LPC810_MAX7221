#ifndef _MAX7221LIB_H_
#define _MAX7221LIB_H_

#include "LPC8xx.h"

// define max7221 registers
#define max7221_reg_noop         0x00
#define max7221_reg_digit0       0x01
#define max7221_reg_digit1       0x02
#define max7221_reg_digit2       0x03
#define max7221_reg_digit3       0x04
#define max7221_reg_digit4       0x05
#define max7221_reg_digit5       0x06
#define max7221_reg_digit6       0x07
#define max7221_reg_digit7       0x08
#define max7221_reg_decodeMode   0x09
#define max7221_reg_intensity    0x0a
#define max7221_reg_scanLimit    0x0b
#define max7221_reg_shutdown     0x0c
#define max7221_reg_displayTest  0x0f

#define LED_0       1
#define LED_1       2
#define LED_2       4
#define LED_3       8
#define LED_4       16
#define LED_5       32
#define LED_6       64
#define LED_7       128

#define LOW 0
#define HIGH 1

#define cMATRIX_SNGL	1
#define cMATRIX_RED		1
#define cMATRIX_GREEN	2
#define cMATRIX_BOTH	3

#define MHZ 1000000

void MAX7221_Write( unsigned char reg, unsigned char col);
void MAX7221_WriteToMatrix( unsigned char reg, unsigned char col, unsigned char m);
void MAX7221_WriteBoth( unsigned char reg, unsigned char col);
//
void MAX7221_Init ( unsigned char m );
void MAX7221_Shutdown ( unsigned char m);
void MAX7221_WakeUp ( unsigned char m);
void MAX7221_Intensity ( unsigned char i, unsigned char m);

#endif
