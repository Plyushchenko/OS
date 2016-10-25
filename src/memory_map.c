#include <memory_map.h>

extern const uint32_t multiboot_info;
extern char text_phys_begin[];
extern char bss_phys_end[];
char PRINT_MULTIBOOT_MEMORY_AVAILABLE[] = "MULTIBOOT_MEMORY_AVAILABLE";
char PRINT_MULTIBOOT_MEMORY_RESERVED[] = "MULTIBOOT_MEMORY_RESERVED";

struct memory_chunk available_chunks[MAX_CHUNKS];
uint32_t chunks_number = 0;

void add_to_available_chunks(uint64_t l, uint64_t r)
{
	if (l > r)
	{
		return;
	}
	available_chunks[chunks_number].l = l;
	available_chunks[chunks_number++].r = r;
	printf("[0x%llx, 0x%llx] type = MULTIBOOT_MEMORY_AVAILABLE\n", l, r);
}

void sort_available_chunks(void)
{
	for (uint32_t i = 0; i < chunks_number; i++)
	{
		for (uint32_t j = i + 1; j < chunks_number; j++)
		{
			if (available_chunks[i].l > available_chunks[j].l)
			{
				struct memory_chunk tmp = available_chunks[i];
				available_chunks[i] = available_chunks[j];
				available_chunks[j] = tmp;
			}
		}
	}
}
void parse_memory_map(void) 
{
    multiboot_info_t* mbi = (multiboot_info_t*)multiboot_info;

   	if (((mbi -> flags) & (1 << 6)) == 0)
	{
		return; //no memory map
	} 

	uint64_t kernel_begin = (uint64_t)text_phys_begin;
	uint64_t kernel_end = (uint64_t)bss_phys_end;
	printf("[0x%llx, 0x%llx] type = KERNEL\n", kernel_begin, kernel_end); 	
		
	for (multiboot_memory_map_t* mmap = (mbi -> mmap_addr); 
		mmap < (mbi -> mmap_addr) + (mbi -> mmap_length); 
		mmap = (multiboot_memory_map_t*)((uint32_t)mmap + (mmap -> size) + 4))
	{
		uint64_t l = (mmap -> addr);
		uint64_t r = l + (mmap -> len) - 1;
		uint32_t type = (mmap -> type);
		if (type == MULTIBOOT_MEMORY_AVAILABLE)
		{
			if (l <= kernel_begin && kernel_end <= r)
			{
				add_to_available_chunks(l,kernel_begin - 1);
				add_to_available_chunks(kernel_end + 1, r);
			}			
			else if (l <= kernel_begin && r <= kernel_end)
			{
				add_to_available_chunks(l,kernel_begin - 1);
			}
			else if (kernel_begin <= l && kernel_end <= r)
			{
				add_to_available_chunks(kernel_end + 1, r);
			}
			else
			{
				add_to_available_chunks(l, r);
			}
		}
		else
		{
			printf("[0x%llx, 0x%llx] type = MULTIBOOT_MEMORY_RESERVED\n", l, r);

		}
	}
	
	for (uint32_t i = 0; i < 80; i++, printf("_"));
	printf("\n");

	sort_available_chunks();
	init_buddy_allocator(available_chunks, chunks_number);
//	buddy_init(available_chunks, chunks_number);

}
