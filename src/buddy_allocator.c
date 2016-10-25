#include <buddy_allocator.h>

uint32_t pages_number;

uint64_t get_length(struct memory_chunk mc)
{
	return mc.r + 1 - mc.l;
}

/*uint32_t MAX_LEVEL = 0;
void get_MAX_LEVEL(uint32_t x)
{
	while (x)
	{
		x /= 2;
		MAX_LEVEL++;
	}
}
*/
struct buddy_descriptor_page* buddy_pages = null;

struct buddy_descriptor_page* free_pages_list[MAX_LEVEL + 1];

void add_buddy_page(uint32_t i, uint32_t level)
{
	buddy_pages[i].next = free_pages_list[level];
	free_pages_list[level] = &buddy_pages[i];
}

uint32_t delete_buddy_page(uint32_t level) //returns number
{
	uint32_t res = (free_pages_list[level] -> number);
	free_pages_list[level] = free_pages_list[level] -> next;
	return res; 
}

void init_buddy_allocator(struct memory_chunk available_chunks[], uint32_t chunks_number) 
{
	pages_number = available_chunks[chunks_number - 1].r / PAGE_SIZE;
//	get_MAX_LEVEL(pages_number);

	uint32_t descriptor_pages_memory = (pages_number) * sizeof(struct buddy_descriptor_page);// + (MAX_LEVEL + 1) * sizeof(struct buddy_descriptor_page*);
	//find a chunk for buddy descriptor pages and free pages list in first 4GB	
	for (uint32_t i = 0; i < chunks_number; i++) 
	{
		if (get_length(available_chunks[i]) >= descriptor_pages_memory && available_chunks[i].r < FOUR_GB) 
		{	
			buddy_pages = (struct buddy_descriptor_page*)(available_chunks[i].l + SHIFTED_BASE);
			printf("[0x%llx, 0x%llx] type = BUDDY_DESCRIPTOR_PAGES\n", available_chunks[i].l, available_chunks[i].l + descriptor_pages_memory - 1);
			available_chunks[i].l += descriptor_pages_memory;
			break;
		}
	}

	if (buddy_pages == null)
	{
		return; //no buddy_pages
	}	

	//memory for free pages list begins where memory for buddy pages ends
//	free_pages_list = (struct buddy_descriptor_page**) (&buddy_pages[pages_number]);
	for (uint32_t i = 0; i <= MAX_LEVEL; i++)
	{
		free_pages_list[i] = null;
	}

	for (uint32_t i = 0; i < pages_number; i++) 
	{
		buddy_pages[i].level = NO_PAGE;
		buddy_pages[i].is_free = 0;
		buddy_pages[i].number = i;
	}

	// align chunks by PAGE_SIZE
	for (uint32_t i = 0; i < chunks_number; i++) 
	{
		uint64_t l = available_chunks[i].l;
		uint64_t r = available_chunks[i].r;
		uint64_t page_begin = l + (PAGE_SIZE - (l % PAGE_SIZE)) % PAGE_SIZE; // page_begin should be divisible by PAGE_SIZE
		uint32_t num = ((uint32_t)(page_begin / PAGE_SIZE));
		while (page_begin + PAGE_SIZE <= r) 
		{
			buddy_pages[num].level = 0;
			buddy_pages[num++].is_free = 1;
			page_begin += PAGE_SIZE;
		} 
	}

	//merge as much as possjble    
	for (uint32_t j = 0; j < MAX_LEVEL; j++) 
	{
		for (uint32_t i = 0; i + (1 << (j + 1)) < pages_number; i += (1 << (j + 1))) 
		{
			if (buddy_pages[i].level != j || !buddy_pages[i].is_free)
			{
				continue;
			}
			uint32_t buddy = GET_BUDDY(i, j);
			if (buddy >= pages_number || buddy_pages[buddy].level != j || !buddy_pages[buddy].is_free)
			{
				continue;
			}
			//merge with buddy, set buddy unfree			
			buddy_pages[i].level = j + 1;
			buddy_pages[buddy].is_free = 0;               
		}
	}

	for (uint32_t i = 0; i < pages_number; i++) 
	{
		if (buddy_pages[i].level != NO_PAGE && buddy_pages[i].is_free) 
		{
//			printf("%u %u\n", i, buddy_pages[i].level);			
			add_buddy_page(i, buddy_pages[i].level);           
		}
	}
}
	
void make_free_pages_list_nonempty(uint32_t level)
{
	if (level > MAX_LEVEL || free_pages_list[level] != null)
	{
		return;
	}
	make_free_pages_list_nonempty(level + 1);
	if (free_pages_list[level + 1] == null)
	{
		return; // no free page of higher level
	}

	uint32_t i = delete_buddy_page(level + 1);
	uint32_t buddy = GET_BUDDY(i, level);

	buddy_pages[i].level = level;
	buddy_pages[buddy].level = level;
	buddy_pages[buddy].is_free = 1;

	add_buddy_page(i, level);
	add_buddy_page(buddy, level);
} 
uint64_t buddy_alloc(uint32_t level)
{
	make_free_pages_list_nonempty(level);
	uint32_t res = delete_buddy_page(level);
//	printf("res = %u\n", res);
	uint64_t tmp = SHIFTED_BASE;	//overflow "res * PAGE_SIZE + SHIFTED_BASE"
	return ((uint64_t)res) * PAGE_SIZE + tmp;
}

void buddy_free(uint64_t address)
{
	uint32_t i = (address - SHIFTED_BASE) / PAGE_SIZE;
	buddy_pages[i].is_free = 1;

	//merge with buddy; if this page has less number then buddy's then swap; go to next level
	for (uint32_t j = buddy_pages[i].level; j < MAX_LEVEL; j++)
	{
		uint32_t buddy = GET_BUDDY(i, j);
		if (buddy >= pages_number || buddy_pages[buddy].level != j || !buddy_pages[buddy].is_free)
		{
			break;
		}
		if (buddy < i)
		{
			uint32_t tmp = i;
			i = buddy;
			buddy = tmp;
		}		
		buddy_pages[i].level = j + 1;			
		buddy_pages[buddy].is_free = 0;
	}
	add_buddy_page(i, buddy_pages[i].level);
}
