#include <desc.h>

void make_desc_table_entry(uint64_t offset, uint16_t type, struct desc_table_entry* entry){
	//"& 0xffff" gets last 16 bits
	entry -> data[0] = (offset & 0xffff) | (KERNEL_CS << 16);
	// "1 << 7" sets P bit
	entry -> data[1] = (((offset >> 16) & 0xffff) << 16) | (type << 8) | (1 << 15);
	entry -> data[2] = (offset >> 32);
	entry -> data[3] = 0;
}

int is_exception(uint64_t x)
{
	return (x == 8) || (10 <= x && x <= 14) || (x == 17);
}

/*void print_two_digits(uint64_t x)
{
	char a = x / 10 + '0';
	write_char_to_serial(a);
	a = x % 10 + '0';
	write_char_to_serial(a);
}
*/

char EXCEPTION[] = "exception";
char JUST_INTERRUPTION[] = "just interruption";

void interrupt_handler(uint64_t interrupt_vector, uint64_t error_code)
{
	if (is_exception(interrupt_vector))
	{
		printf("%s %lld", EXCEPTION, error_code);
//		write_string_to_serial(EXCEPTION);
//		print_two_digits(error_code);
	}
	else
	{
		printf("%s", JUST_INTERRUPTION);	
//		write_string_to_serial(JUST_INTERRUPTION);
	}
	printf("%lld", interrupt_vector);	
//	print_two_digits(interrupt_vector);
	
	//EOI	
	if (((uint64_t)32) <= interrupt_vector && interrupt_vector < ((uint64_t)40))
	{
		out8(MASTER_PIC_COMMAND_PORT, 1 << 5);
	}
	if (((uint64_t)40) <= interrupt_vector && interrupt_vector < ((uint64_t)48))
	{
		out8(MASTER_PIC_COMMAND_PORT, 1 << 5);
		out8(SLAVE_PIC_COMMAND_PORT, 1 << 5);
	}

}

static struct desc_table_entry desc_table[DESC_NUMBER];
extern uint64_t handler_wrappers[];

void init_idt(void)
{
	for (int i = 0; i < DESC_NUMBER; i++)
		make_desc_table_entry(handler_wrappers[i], 14, &desc_table[i]);

	struct desc_table_ptr ptr = {sizeof(desc_table) - 1, (uint64_t)(&desc_table)};
	write_idtr(&ptr);
		
}

void init_pic(void)
{
	out8(MASTER_PIC_COMMAND_PORT, (1 << 4) | (1 << 0));
	out8(MASTER_PIC_DATA_PORT, 32);
	out8(MASTER_PIC_DATA_PORT, 1 << 2);
	out8(MASTER_PIC_DATA_PORT, 1);
	out8(MASTER_PIC_DATA_PORT, 255);

	out8(SLAVE_PIC_COMMAND_PORT, (1 << 4) | (1 << 0));
	out8(SLAVE_PIC_DATA_PORT, 40);
	out8(SLAVE_PIC_DATA_PORT, 2);
	out8(MASTER_PIC_DATA_PORT, 1);
	out8(SLAVE_PIC_DATA_PORT, 255);

}
