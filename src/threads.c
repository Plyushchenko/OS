#include <threads.h>

void lock()
{
	__asm__ volatile("cli");
}

void unlock()
{
	__asm__ volatile("sti");
}
