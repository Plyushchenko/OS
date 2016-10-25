#ifndef __MEMORY_MAP_H_
#define __MEMORY_MAP_H_

#include <multiboot.h>
#include <stdint.h>
#include <io.h>

void parse_memory_map(void);

struct memory_chunk
{
	uint64_t l, r;
};

#define MAX_CHUNKS 1000

#endif /* __MEMORY_MAP_H__ */
