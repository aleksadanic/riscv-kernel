#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

void* mem_alloc (size_t size) {
    // printString("mem_alloc\n");
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    // printString("mem_free\n");
    return (int) syscallWrapper (0x02, (uint64) address);
}

int thread_create (TCB** handle, void (*start_routine) (void*), void* arg) {
    // printString("thread_create\n");
    char* stack = (char*) mem_alloc (DEFAULT_STACK_SIZE);
    if (!stack) {
        return -1;
    }
    return (int) syscallWrapper (0x11, (uint64) handle, (uint64) start_routine, (uint64) arg, (uint64) (stack + DEFAULT_STACK_SIZE));
}

int thread_exit () {
    // printString("thread_exit\n");
    return (int) syscallWrapper (0x12);
}

void thread_dispatch () {
    // printString("thread_dispatch\n");
    syscallWrapper (0x13);
}

int sem_open (SCB** handle, unsigned init) {
    // printString("sem_open\n");
    return (int) syscallWrapper (0x21, (uint64) handle, (uint64) init);
}

int sem_close (SCB* handle) {
    // printString("sem_close\n");
    return (int) syscallWrapper (0x22, (uint64) handle);
}

int sem_wait (SCB* id) {
    // printString("sem_wait\n");
    return (int) syscallWrapper (0x23, (uint64) id);
}

int sem_signal (SCB* id) {
    // printString("sem_signal\n");
    return (int) syscallWrapper (0x24, (uint64) id);
}

int sem_wait_n (SCB* id, unsigned n) {
    // printString("sem_wait_n\n");
    return (int) syscallWrapper (0x25, (uint64) id, (uint64) n);
}

int sem_signal_n (SCB* id, unsigned n) {
    // printString("sem_signal_n\n");
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