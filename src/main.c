#include <desc.h>
#include <ioport.h>
#include <ints.h>
#include <pit.h>

static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;
	while (wait);
#endif
}

void main(void)
{
	disable_ints();	
	qemu_gdb_hang();
	init_serial_port();
	init_idt();				
	init_pic();	
//	init_pit();
	
//	out8(MASTER_PIC_DATA_PORT, 254); //binary(11111110), enable interruptions master
//	out8(SLAVE_PIC_DATA_PORT, 255);
	enable_ints(); 
	__asm__("int $3");

	while (1);
}


