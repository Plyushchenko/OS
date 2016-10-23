#include <pit.h>
#include <desc.h>
void init_pit(void)
{
	out8(PIT_COMMAND_PORT, (1 << 5) | (1 << 4) | (1 << 2)); // 2 bytes for coef (4th and 5th); type 2
	out8(PIT_DATA_PORT, 0xae);// 1193182 = 41 * 29102; 29102 = 0x71ae 
	out8(PIT_DATA_PORT, 0x71); 
}
