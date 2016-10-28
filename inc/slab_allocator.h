#ifndef __SLAB_ALLOCATOR__
#define __SLAB_ALLOCATOR__

#include <stdint.h>
#include <memory.h>
#define BLOCKS_FOR_ALLOCATION 16 //smth like magic constant

struct slab_descriptor
{
	struct slab_descriptor* next;
};	
struct slab_allocator
{
	struct slab_descriptor* descriptor;
	uint32_t block_size;
};
struct slab_descriptor* make_slab_descriptors(uint32_t block_size);
struct slab_allocator* make_slab_allocator(uint32_t block_size);
void* slab_alloc(struct slab_allocator* allocator);

/*//void* slab_alloc(struct slab_descriptor** descriptor_address);
void* slab_alloc(struct slab_descriptor* descriptor);

void slab_free(struct slab_descriptor** descriptor_address, void* address);
*/
#endif /* __SLAB_ALLOCATOR__ */
