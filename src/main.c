#include <desc.h>
#include <ioport.h>
#include <ints.h>
#include <pit.h>
#include <io.h>
#include <stdint.h>
#include <memory_map.h>

/*
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;
	while (wait);
#endif
}*/

void main(void)
{
	disable_ints();	
//	qemu_gdb_hang();
	init_serial_port();
	init_idt();				
	init_pic();	
	init_pit();
	
//	out8(MASTER_PIC_DATA_PORT, 254); //binary(11111110), enable interruptions master
//	enable_ints(); 

	parse_memory_map();
	
//	while (1);
	return;
}



