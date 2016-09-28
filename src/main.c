static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;
	while (wait);
#endif
}

#include <desc.h>
#include <ioport.h>

void interruptions_off()
{
	__asm__ volatile ("cli");
}
void interruptions_on()
{
	__asm__ volatile ("sti");
}
void main(void)
{
	interruptions_off();	
	qemu_gdb_hang();
	init_serial_port();
	init_idt();				
	init_pic();	
	interruptions_on(); 
	__asm__("int $3");


	while (1);
}


