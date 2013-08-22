#ifndef _MAX7221GRAPH_H_
#define _MAX7221GRAPH_H_

void MAX7221_ScrollText( char *s, unsigned char m);
void MAX7221_CustomFigure( char *dt, unsigned char m);

#define FONT8x8     1
#define cFIGURES	1

#ifdef cFIGURES
	#define ARROW_LFT   0
	#define ARROW_RGT   1
	#define ARROW_UP    2
	#define ARROW_DWN   3
	#define SMILE	    4
	//
	void MAX7221_Figure( unsigned char f, unsigned char m);
#endif

#define cMAX_STR_LEN	32

#endif
