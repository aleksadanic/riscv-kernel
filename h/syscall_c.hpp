#include "../lib/hw.h"

uint64 syscallWrapper (uint64 a0 = 0, uint64 a1 = 0, uint64 a2 = 0, uint64 a3 = 0);

void* mem_alloc (size_t size);
int mem_free (void* address);