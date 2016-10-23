#include <memory_map.h>

extern const uint32_t multiboot_info;

void print_memory_map(void) 
{
    multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;

   	if ((mbi -> flags) & (1 << 6) == 0)
	{
		return; //no memory map
	} 

	for (multiboot_memory_map_t *mmap = (mbi -> mmap_addr); 
		mmap < (mbi -> mmap_addr) + (mbi -> mmap_length); 
		mmap = (multiboot_memory_map_t *)((uint32_t)mmap + (mmap -> size) + 4))
	{
		uint64_t l = (mmap -> addr);
		uint64_t r = l + (mmap -> len) - 1;
		uint32_t type = (mmap -> type);        
		printf("[0x%llx, 0x%llx] type = %u\n", l, r, type);
    }
}
