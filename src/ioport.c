#include <ioport.h>

void init_serial_port() 
{
	out8(SERIAL_PORT + 3, 0x80);	// set 7th bit for div coef
	out8(SERIAL_PORT + 1, 0x00);    
	out8(SERIAL_PORT + 0, 0x01);    // set coef as 0x0001

	out8(SERIAL_PORT + 3, 0x07);    // set frame format as 8 bits in frame and 1 stop bit (0000'0111)
}

void write_char_to_serial(char c)
{
	while (!(in8(SERIAL_PORT + 5) & 0x20)); // waiting 'til 5th bit of 5th byte get set 
	out8(SERIAL_PORT, c);
}

void write_string_to_serial(char *s)
{
	while (*s)
	{
		write_char_to_serial(*(s++));
	}
}


