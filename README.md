LPC810_CodeBase
===============

LPC810 driving a MAX7221 and a dual color matrix [1]
The software can receive a string from the serial port (230400,n,8,1):
Fields structure
 0           1
<payload[32],m>
where m can be:
m -> 	a single ASCII character that can be:
	1: payload is a string, max 32 char, to show using the RED matrix
	2: same as opz. 1 but to show using the GREEN one
	3: the first character inside the payload is the index of a preloaded figure, (see max7221_graph.h)
	   the second character is the color: red o green.
	4: using this option, you can send a custom figure. The payload is 17 byte.
	   The first 16 bytes are the ASCII version of the 8bit to show. The last byte the color.
	5: put the MCU and MAX7221 in deepsleep.
payload[32]	->	the string to show or the custom figure.


Open source code base for the ARM Cortex M0+ LPC810 family from NXP.

This code base is intended to work with the LPC810 in a DIP8 package.  
While these drivers are based on the original LPC800 example code from NXP, the LPC810 has limited resources 
(4KB flash and 1KB SRAM), so smaller, lighter-weight drivers had to be written to get the most out of 
these resources we have.

The current code implements the following peripheral drivers:

- A basic SPI driver
- Some simple GPIO helper functions (although GPIO should normally be accessed directly via the appropriate registers)
- A simple driver for UART0 and printf-redirection that allows 'printf' output to be transmitted to UART0
- A basic multi-rate timer driver that allows us to set delays

The code base also implements a mini printf that takes up much less space than the default printf used in most 
libc variants.  If necessary, it's easy to change the printf redirection to a location other than UART0 via the printf-redirection.c file.