#include <slab_allocator.h>

uint32_t get_buddy_page_level(uint32_t x)
{
	x *= BLOCKS_FOR_ALLOCATION;
	uint64_t allocated_memory = PAGE_SIZE;
	uint32_t level = 0;
	while (allocated_memory < x)
	{
		allocated_memory *= 2;
		level++;
	}	
	return level;
}

struct slab_descriptor* make_slab_descriptors(uint32_t block_size)
{
	struct slab_descriptor* descriptors;
	struct slab_descriptor* now;
	descriptors = now = (struct slab_descriptor*)buddy_alloc(get_buddy_page_level(block_size + sizeof(struct slab_descriptor)));	
	//split a plain piece of memory into structure (picture above)
	for (uint32_t i = 0; i < BLOCKS_FOR_ALLOCATION; i++)
	{
		if (i + 1 != BLOCKS_FOR_ALLOCATION)
		{
			now -> next = (struct slab_descriptor*)((char *)(now + block_size + sizeof(struct slab_descriptor)));
		}
		else
		{
			now -> next = null;
		}
		now = now -> next;
	}
	return descriptors;
}
struct slab_allocator* make_slab_allocator(uint32_t block_size)
{
	//hope we can afford 4kb for each allocator
	//sizeof(slab_allocator) < PAGE_SIZE
	struct slab_allocator* allocator = (struct slab_allocator*)(buddy_alloc(0));
	allocator -> block_size = block_size;
	allocator -> descriptor = make_slab_descriptors(block_size);
	printf("make allocator: 0x%llx; 0x%llx\n", &allocator, allocator -> descriptor);	
	return allocator;
}
void* slab_alloc(struct slab_allocator* allocator)
{
	if ((allocator -> descriptor) == null)
	{
		allocator -> descriptor = make_slab_descriptors(allocator -> block_size);
	}
	void* res = (void*)(allocator -> descriptor);
	(allocator -> descriptor) = (allocator -> descriptor) -> next;
	return res;
}

void slab_free(struct slab_allocator* allocator, void* address)
{
	struct slab_descriptor* new_descriptor = (struct slab_descriptor*)(address - sizeof(struct slab_descriptor*));
	new_descriptor -> next = (allocator -> descriptor);
	(allocator -> descriptor) = new_descriptor;		
}
