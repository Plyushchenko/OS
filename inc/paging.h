#ifndef __PAGING_H__
#define __PAGING_H__

#include <memory.h>
#include <stdint.h>
#include <memory_map.h>

#define TABLE_ENTRIES 512
#define AFTER_HOLE 256

//from bootstap.S
#define PTE_PRESENT	(1 << 0)
#define PTE_WRITE	(1 << 1)
#define PTE_LARGE	(1 << 7)

void init_paging(void);
#endif /* __PAGING_H__ */
