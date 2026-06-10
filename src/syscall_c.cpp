#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

uint64 syscallWrapper (uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    printString("syscallWrapper\n");
    asm volatile (
        "mv a0, %[a0]\n"
        "mv a1, %[a1]\n"
        "mv a2, %[a2]\n"
        "mv a3, %[a3]\n"
        "ecall\n"
        "mv %[a0], a0"
        : [a0] "+r"(a0)
        : [a1] "r"(a1), [a2] "r"(a2), [a3] "r"(a3)
        : "a1", "a2", "a3", "memory"
    );
    return a0;
}

void* mem_alloc (size_t size) {
    printString("mem_alloc\n");
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    return (int) syscallWrapper (0x02, (uint64) address);
}