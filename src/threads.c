#include <threads.h>
#include <ints.h>

void lock()
{
	disable_ints();
}

void unlock()
{
	enable_ints();
}
