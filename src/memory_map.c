#include <memory_map.h>

extern const uint32_t multiboot_info;
extern char text_phys_begin[];
extern char bss_phys_end[];
char PRINT_MULTIBOOT_MEMORY_AVAILABLE[] = "MULTIBOOT_MEMORY_AVAILABLE";
char PRINT_MULTIBOOT_MEMORY_RESERVED[] = "MULTIBOOT_MEMORY_RESERVED";

void print_memory_map(void) 
{
    multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;

   	if ((mbi -> flags) & (1 << 6) == 0)
	{
		return; //no memory map
	} 

	uint64_t kernel_begin = (uint64_t)text_phys_begin;
	uint64_t kernel_end = (uint64_t)bss_phys_end;
	printf("[0x%llx, 0x%llx] type = KERNEL\n", kernel_begin, kernel_end); 	
		
	for (multiboot_memory_map_t *mmap = (mbi -> mmap_addr); 
		mmap < (mbi -> mmap_addr) + (mbi -> mmap_length); 
		mmap = (multiboot_memory_map_t *)((uint32_t)mmap + (mmap -> size) + 4))
	{
		uint64_t l = (mmap -> addr);
		uint64_t r = l + (mmap -> len) - 1;
		printf("[0x%llx, 0x%llx] ", l, r);
		if ((mmap -> type) == MULTIBOOT_MEMORY_AVAILABLE)
		{
			printf("type = MULTIBOOT_MEMORY_AVAILABLE\n");
		}
		else
		{
			printf("type = MULTIBOOT_MEMORY_RESERVED\n");
		}

    }
}
