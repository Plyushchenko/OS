#include <desc.h>
#include <ioport.h>
#include <ints.h>
#include <pit.h>
#include <io.h>
#include <stdint.h>
#include <memory_map.h>
#include <buddy_allocator.h>

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
//	uint64_t a = buddy_alloc(11);
//	uint64_t b = buddy_alloc(11);
//	uint64_t c = buddy_alloc(11);
//0x7fd8000 0x7aac000 0x10c000
//0xffff800007fd8000

//	printf("????????????????????0x%llx 0x%llx 0x%llx\n", a, b, c);
//	buddy_free(c);
//	uint64_t d = buddy_alloc(12);
//	printf("%llx\n", FOUR_GB);
//	struct memory_chunk* ptr = available_chunks;

//	while (1);
	return;
}



