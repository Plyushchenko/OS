#include <paging.h>

extern uint64_t* bootstrap_pml4;
uint64_t* pml4_entries[TABLE_ENTRIES];

extern struct memory_chunk available_chunks[];
extern uint32_t chunks_number;
extern uint64_t memory_map_end;

void init_paging(void)
{
	//memory map: [0..memory_map_end] => we have (memory_map_end + 1) blocks
	// (a + b - 1) / b = ceil(a / b)
	uint64_t GB_to_map = (memory_map_end + GB) / GB; 
	uint64_t entries_to_map = (GB_to_map + TABLE_ENTRIES - 1) / TABLE_ENTRIES;
	printf("GB_to_map = %llu, entries_to_map = %llu\n", GB_to_map, entries_to_map);

	//find a chunk for pml4 entries in first 4GB	
	for (uint32_t i = 0; i < chunks_number; i++)
	{
		uint64_t l = available_chunks[i].l + (PAGE_SIZE - (available_chunks[i].l % PAGE_SIZE)) % PAGE_SIZE; // page_begin should be divisible by PAGE_SIZE
		uint64_t r = available_chunks[i].r;		
		if (r > FOUR_GB)
		{
			r = FOUR_GB;
		}
		if ((l > r) || ((r + 1 - l) < (entries_to_map * PAGE_SIZE)))
		{
			continue;
		}

		for (uint64_t j = 0; j < entries_to_map; j++, l += PAGE_SIZE)
		{
			pml4_entries[j] = (uint64_t*)l;
		}
		available_chunks[i].l = l + entries_to_map * PAGE_SIZE; 

		break;
	}

	//put every GB at pml4 entry [..., |HOLE|, 256:[1st..512nd], 257:[513rd..1024th], ...]
	for (uint64_t i = 0; i < GB_to_map; i++)
	{
		pml4_entries[i / TABLE_ENTRIES + AFTER_HOLE][i % TABLE_ENTRIES] = (i * GB) | PTE_PRESENT | PTE_WRITE | PTE_LARGE;
	}

	for (uint64_t i = 0; i < entries_to_map; i++)
	{
		bootstrap_pml4[i] = (*pml4_entries[i]) | PTE_PRESENT | PTE_WRITE;
	}

}
