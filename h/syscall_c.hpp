#include "../lib/hw.h"

uint64 syscallWrapper (uint64 a0 = 0, uint64 a1 = 0, uint64 a2 = 0, uint64 a3 = 0, uint64 a4 = 0);

void* mem_alloc (size_t size);
int mem_free (void* address);

class Thread;
typedef Thread* thread_t;
int thread_create (thread_t* handle, void (*start_routine) (void*), void* arg);
int thread_exit ();
void thread_dispatch ();
int thread_adopt (thread_t* handle);