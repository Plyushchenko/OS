#include <slab_allocator.h>

uint32_t get_buddy_page_level(uint32_t x)
{
	uint64_t allocated_memory = PAGE_SIZE;
	uint32_t level = 0;
	while (allocated_memory < x)
	{
		allocated_memory *= 2;
		level++;
	}	
	return level;
}

void split_to_descriptors(struct slab_descriptor* now, uint32_t block_size)
{
	for (uint32_t i = 0; i < BLOCKS_FOR_ALLOCATION - 1; i++)
	{
		now -> next = (struct slab_descriptor*)((char *)(now + block_size + sizeof(struct slab_descriptor)));
		now = now -> next;
	}
	now -> next = null;
}

struct slab_allocator* make_slab_allocator(uint32_t block_size)
{
	//allocate space for allocator and descriptors
	uint32_t tmp = sizeof(struct slab_allocator) + BLOCKS_FOR_ALLOCATION * (block_size + sizeof(struct slab_descriptor));
	struct slab_allocator* allocator = (struct slab_allocator*)buddy_alloc(get_buddy_page_level(tmp));	
	allocator -> block_size = block_size;
	allocator -> descriptor = (struct slab_descriptor*)((char *)(allocator + sizeof(struct slab_allocator)));
	split_to_descriptors(allocator -> descriptor, allocator -> block_size);
	return allocator;
}

void* slab_alloc(struct slab_allocator* allocator)
{
	if ((allocator -> descriptor) == null)
	{
		//allocate space only for descriptors	
		uint32_t tmp = BLOCKS_FOR_ALLOCATION * ((allocator -> block_size) + sizeof(struct slab_descriptor));
		struct slab_descriptor* descriptor = (struct slab_descriptor*)buddy_alloc(get_buddy_page_level(tmp));	
		allocator -> descriptor = descriptor;
		split_to_descriptors(allocator -> descriptor, allocator -> block_size);
	}
	void* res = (void*)(allocator -> descriptor);
	allocator -> descriptor = (allocator -> descriptor) -> next;
	return res;
}

void slab_free(struct slab_allocator* allocator, void* address)
{
	struct slab_descriptor* new_descriptor = (struct slab_descriptor*)(address - sizeof(struct slab_descriptor*));
	new_descriptor -> next = (allocator -> descriptor);
	(allocator -> descriptor) = new_descriptor;		
}	
