#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

uint64 syscallWrapper (uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {
    // printString("syscallWrapper\n");
    asm volatile (
        "mv a0, %[a0]\n"
        "mv a1, %[a1]\n"
        "mv a2, %[a2]\n"
        "mv a3, %[a3]\n"
        "mv a4, %[a4]\n"
        "ecall\n"
        "mv %[a0], a0"
        : [a0] "+r" (a0)
        : [a1] "r" (a1), [a2] "r" (a2), [a3] "r" (a3), [a4] "r" (a4)
        : "a1", "a2", "a3", "a4", "memory"
    );
    return a0;
}

void* mem_alloc (size_t size) {
    // printString("mem_alloc\n");
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    // printString("mem_free\n");
    return (int) syscallWrapper (0x02, (uint64) address);
}

int thread_create (Thread** handle, void (*start_routine) (void*), void* arg) {
    // printString("thread_create\n");
    char* stack = new char[DEFAULT_STACK_SIZE];
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

int sem_open (Semaphore** handle, unsigned init) {
    // printString("sem_open\n");
    return (int) syscallWrapper (0x21, (uint64) handle, (uint64) init);
}

int sem_close (Semaphore* handle) {
    // printString("sem_close\n");
    return (int) syscallWrapper (0x22, (uint64) handle);
}

int sem_wait (Semaphore* id) {
    // printString("sem_wait\n");
    return (int) syscallWrapper (0x23, (uint64) id);
}

int sem_signal (Semaphore* id) {
    // printString("sem_signal\n");
    return (int) syscallWrapper (0x24, (uint64) id);
}

int sem_wait_n (Semaphore* id, unsigned n) {
    // printString("sem_wait_n\n");
    return (int) syscallWrapper (0x25, (uint64) id, (uint64) n);
}

int sem_signal_n (Semaphore* id, unsigned n) {
    // printString("sem_signal_n\n");
    return (int) syscallWrapper (0x26, (uint64) id, (uint64) n);
}

int time_sleep (time_t time) {
    return (int) syscallWrapper (0x31, (uint64) time);
}

void* operator new (size_t size) {
    return mem_alloc (size);
}

void* operator new[] (size_t size) {
    return mem_alloc (size);
}

void operator delete (void* address) {
    mem_free (address);
}

void operator delete[] (void* address) {
    mem_free (address);
}