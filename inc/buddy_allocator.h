#ifndef __BUDDY_ALLOCATOR__
#define __BUDDY_ALLOCATOR__

#include <memory.h>
#include <memory_map.h>

#define null 0
#define NO_PAGE 0xffffffff // level = -1 is for unaccesible page
#define GET_BUDDY(number, level) (number ^ (1 << level))

void init_buddy_allocator(struct memory_chunk available_chunks[], uint32_t chunks_number);
uint64_t buddy_alloc(uint32_t level);
void buddy_free(uint64_t address);

struct buddy_descriptor_page
{
	struct buddy_descriptor_page* next;
	uint32_t level;
	uint8_t is_free;
	uint64_t number;
};

#endif /* __BUDDY_ALLOCATOR__ */
