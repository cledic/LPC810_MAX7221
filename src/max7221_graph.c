#include <string.h>
#include "max7221_lib.h"
#include "max7221_graph.h"
#include "max7221_figure.h"
#include "systick.h"

// Buffer
unsigned char buff[cMAX_STR_LEN*8];

/** Show a scrolling msg
 *
 * @param   char *s     pointer to the msg. (NULL terminating)
 * @return  none
 */
void MAX7221_ScrollText( char *s, unsigned char m)
{
    unsigned int i, ii, sl, sb;
    char ss;
    unsigned char *ptr;
    unsigned long tmr;

    if ( strlen( s) >= cMAX_STR_LEN)
        return;

    // Get the lenght of the msg...
    sl = strlen( s);

    // ...get the lenght as byte
    sb = sl * 8;

    // clear the buffer
    for( i=0; i<cMAX_STR_LEN*8; i++)
    	buff[i]=0;

    ptr = &buff[0];

    //
    for (ii=0; ii<sl; ii++) {
        ss=*s;
        // check if in range.
        if ( ss >= ' ' && ss <= '~') {
        	ss-=' ';
        	//
			for ( i=0; i<7; i++) {
				*ptr=f8x8fv[(unsigned char)ss][i];
				ptr++;
			}
        }
        s++;
    }
    //
    MAX7221_Init( m);
    //
    for ( i=0; i<sb; i++) {
    	MAX7221_Write( 1, buff[i]);
    	MAX7221_Write( 2, buff[i+1]);
    	MAX7221_Write( 3, buff[i+2]);
    	MAX7221_Write( 4, buff[i+3]);
    	MAX7221_Write( 5, buff[i+4]);
    	MAX7221_Write( 6, buff[i+5]);
    	MAX7221_Write( 7, buff[i+6]);
    	MAX7221_Write( 8, buff[i+7]);

    	tmr=getTimerTick();
        while( getTimerTickDiff( tmr) < 5);
    }
}

#ifdef cFIGURES
/** Show a predefined figure
 *
 * @param unsigned char f   the figure to visualize:
 *                          ARROW_LFT, ARROW_RGT, ARROW_UP, ARROW_DWN, SMILE
 * @param unsigned char m	The color matrix to use: red or green.
 *
 * @return none
 */
void MAX7221_Figure( unsigned char f, unsigned char m)
{
    unsigned int i;

    if ( f>=sizeof(readyMadeFigure)/sizeof(readyMadeFigure[0]))
        f=0;

    //
    MAX7221_Init( m);
    //
    for ( i=0; i<8; i++) {
    	MAX7221_Write( i+1, readyMadeFigure[ f][i]);
    }
}
#endif

/** Show a custom figure
 *
 * @param char *dt			The figure to show
 * @param unsigned char m	The color matrix to use: red or green
 *
 * @return none
 */
void MAX7221_CustomFigure( char *dt, unsigned char m)
{
    //
    MAX7221_Init( m);
    //
	MAX7221_Write( 1, dt[0]);
	MAX7221_Write( 2, dt[1]);
	MAX7221_Write( 3, dt[2]);
	MAX7221_Write( 4, dt[3]);
	MAX7221_Write( 5, dt[4]);
	MAX7221_Write( 6, dt[5]);
	MAX7221_Write( 7, dt[6]);
	MAX7221_Write( 8, dt[7]);

}

