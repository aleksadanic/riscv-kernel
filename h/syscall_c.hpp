#include "../lib/hw.h"

uint64 syscallWrapper (uint64 a0 = 0, uint64 a1 = 0, uint64 a2 = 0, uint64 a3 = 0, uint64 a4 = 0);

void* mem_alloc (size_t size);
int mem_free (void* address);

class Thread;
typedef Thread* thread_t;
int thread_create (thread_t* handle, void (*start_routine) (void*), void* arg);
int thread_exit ();
void thread_dispatch ();

class Semaphore;
typedef Semaphore* sem_t;
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
int sem_wait_n (sem_t id, unsigned n);
int sem_signal_n (sem_t id, unsigned n);

typedef unsigned long time_t;
int time_sleep (time_t time);