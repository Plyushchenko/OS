#include <desc.h>
#include <ioport.h>
#include <ints.h>
#include <pit.h>
#include <io.h>
#include <stdint.h>
#include <memory_map.h>
#include <buddy_allocator.h>
#include <paging.h>
#include <slab_allocator.h>
#include <threads.h>

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
	init_paging();
	init_buddy_allocator();
/*
	uint64_t a = buddy_alloc(11);
	uint64_t b = buddy_alloc(11);
	uint64_t c = buddy_alloc(11);
	printf("????????????????????0x%llx 0x%llx 0x%llx\n", a, b, c);
	buddy_free(c);
	uint64_t d = buddy_alloc(12);
	printf("0x%llx\n", d);
	struct slab_allocator *tmp;
	tmp = make_slab_allocator(400);
//	for (uint32_t i = 0; i < BLOCKS_FOR_ALLOCATION - 1; i++)
	printf("main: 0x%llx\n", tmp);		

	void *e;
	for (int i = 0; i < 16; i++)
		e = slab_alloc(tmp);
	slab_free(tmp, e);
	slab_alloc(tmp);
	slab_alloc(tmp);
*/
/**/

//	while (1);
	return;
}



