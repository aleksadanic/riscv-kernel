#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

void* mem_alloc (size_t size) {
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    return (int) syscallWrapper (0x02, (uint64) address);
}

int thread_create (thread_t* handle, void (*start_routine) (void*), void* arg) {
    char* stack = (char*) mem_alloc (DEFAULT_STACK_SIZE);
    if (!stack) {
        return -1;
    }
    return (int) syscallWrapper (0x11, (uint64) handle, (uint64) start_routine, (uint64) arg, (uint64) (stack + DEFAULT_STACK_SIZE));
}

int thread_exit () {
    return (int) syscallWrapper (0x12);
}

void thread_dispatch () {
    syscallWrapper (0x13);
}

int sem_open (sem_t* handle, unsigned init) {
    return (int) syscallWrapper (0x21, (uint64) handle, (uint64) init);
}

int sem_close (sem_t handle) {
    return (int) syscallWrapper (0x22, (uint64) handle);
}

int sem_wait (sem_t id) {
    return (int) syscallWrapper (0x23, (uint64) id);
}

int sem_signal (sem_t id) {
    return (int) syscallWrapper (0x24, (uint64) id);
}

int sem_wait_n (sem_t id, unsigned n) {
    return (int) syscallWrapper (0x25, (uint64) id, (uint64) n);
}

int sem_signal_n (sem_t id, unsigned n) {
    return (int) syscallWrapper (0x26, (uint64) id, (uint64) n);
}

int time_sleep (time_t time) {
    return (int) syscallWrapper (0x31, (uint64) time);
}

char getc () {
    return (char) syscallWrapper (0x41);
}

void putc (char c) {
    syscallWrapper (0x42, (uint64) c);
}