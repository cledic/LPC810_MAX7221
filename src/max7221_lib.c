#include "spi.h"
#include "max7221_lib.h"

#define SPIx	LPC_SPI0
static unsigned char MatrixInUse=0;

void MAX7221_Write( unsigned char reg, unsigned char col)
{
	MAX7221_WriteToMatrix( reg, col, MatrixInUse);
}

void MAX7221_WriteToMatrix( unsigned char reg, unsigned char col, unsigned char m)
{
	unsigned char reg1=0, reg2=0, col1=0, col2=0;

	switch (m) {
	case cMATRIX_RED:
		reg2=reg;
		col2=col;
		break;
	case cMATRIX_GREEN:
		reg1=reg;
		col1=col;
		break;
	case cMATRIX_BOTH:
		reg1=reg2=reg;
		col1=col2=col;
		break;
	default:
		reg1=reg;
		col1=col;
		break;
	}

	while ( (SPIx->STAT & SPI_STAT_TXRDY) == 0 );
	SPIx->TXDATCTL = SPI_TXDATCTL_FSIZE(8-1) | reg1; 	//
	while ( (SPIx->STAT & SPI_STAT_RXRDY) == 0 );
	SPIx->RXDAT;

	while ( (SPIx->STAT & SPI_STAT_TXRDY) == 0 );
	SPIx->TXDATCTL = SPI_TXDATCTL_FSIZE(8-1) | col1;	//
	while ( (SPIx->STAT & SPI_STAT_RXRDY) == 0 );
	SPIx->RXDAT;	//

	while ( (SPIx->STAT & SPI_STAT_TXRDY) == 0 );
	SPIx->TXDATCTL = SPI_TXDATCTL_FSIZE(8-1) | reg2; 	//
	while ( (SPIx->STAT & SPI_STAT_RXRDY) == 0 );
	SPIx->RXDAT;

	while ( (SPIx->STAT & SPI_STAT_TXRDY) == 0 );
	SPIx->TXDATCTL = SPI_TXDATCTL_FSIZE(8-1) | SPI_TXDATCTL_EOT | col2;	//
	while ( (SPIx->STAT & SPI_STAT_RXRDY) == 0 );
	SPIx->RXDAT;	//

}

void MAX7221_WriteBoth( unsigned char reg, unsigned char col)
{

	MAX7221_WriteToMatrix( reg, col, cMATRIX_BOTH);

}

/** Init both MAX7221
 *
 * @param	m can be cMATRIX_RED or cMATRIX_GREEN, the  matrix to use.
 * 			The other one will be configured in shutdown.
 */
void MAX7221_Init ( unsigned char m)
{
    unsigned char e;

    MAX7221_WriteBoth( max7221_reg_scanLimit, 0x07);
    MAX7221_WriteBoth( max7221_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    MAX7221_WriteBoth( max7221_reg_shutdown, 0x01);    // not in shutdown mode
    MAX7221_WriteBoth( max7221_reg_displayTest, 0x00); // no display test

	for ( e=1; e<=8; e++) {    // empty registers, turn all LEDs off
		MAX7221_WriteBoth( e, 0x00);
	}

	MAX7221_WriteBoth( max7221_reg_intensity, 0x0f);
	// range: 0x00 to 0x0f

	if ( m == cMATRIX_GREEN)
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x00, cMATRIX_RED);
	else
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x00, cMATRIX_GREEN);

	MatrixInUse=m;
}

/** Put the MAX7221 in shutdown
 *
 * @param	m can be cMATRIX_RED or cMATRIX_GREEN, the  matrix to put in shutdown.
 *
 */
void MAX7221_Shutdown ( unsigned char m) {

	if ( m == cMATRIX_GREEN)
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x00, cMATRIX_GREEN);
	else
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x00, cMATRIX_RED);

}

void MAX7221_WakeUp ( unsigned char m) {

	if ( m == cMATRIX_GREEN)
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x01, cMATRIX_GREEN);
	else
		MAX7221_WriteToMatrix( max7221_reg_shutdown, 0x01, cMATRIX_RED);

}

void MAX7221_Intensity ( unsigned char i, unsigned char m) {

	MAX7221_WriteToMatrix( max7221_reg_intensity, i&0x0F, m);    // Imposta l'instensità dei LED

}


