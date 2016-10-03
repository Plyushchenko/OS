#ifndef __DESC_H__
#define __DESC_H__

#include <stdint.h>
#include <memory.h>
#include <ioport.h>
#include <io.h>

#define MASTER_PIC_COMMAND_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21
#define SLAVE_PIC_COMMAND_PORT 0xa0
#define SLAVE_PIC_DATA_PORT 0xa1

#define DESC_NUMBER (32 + 16)

struct desc_table_ptr {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

static inline void read_idtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sidt %0" : "=m"(*ptr));
}

static inline void write_idtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lidt %0" : : "m"(*ptr));
}

static inline void read_gdtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sgdt %0" : "=m"(*ptr));
}

static inline void write_gdtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lgdt %0" : : "m"(*ptr));
}

struct desc_table_entry {
	uint32_t data[4];
}__attribute((packed));

void print_two_digits(uint64_t x);
void make_desc_table_entry(uint64_t offset, uint16_t type, struct desc_table_entry* entry);
int is_exception(uint64_t x);
void interrupt_handler(uint64_t interrupt_vector, uint64_t error_code);
void init_idt();
void init_pic();

#endif /*__DESC_H__*/		
