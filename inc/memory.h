#ifndef __MEMORY_H__
#define __MEMORY_H__

#define VIRTUAL_BASE	0xffffffff80000000
#define SHIFTED_BASE    0xffff800000000000
#define PAGE_SIZE	0x1000
#define KERNEL_CS	0x08
#define KERNEL_DS	0x10
#define FOUR_GB ((uint64_t)0x100000000) // (1 << 32)
#define GB      ((uint64_t)0x40000000) //(1 << 30)
#define null 0

#endif /*__MEMORY_H__*/
