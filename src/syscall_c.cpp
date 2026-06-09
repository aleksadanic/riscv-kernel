#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

uint64 syscallWrapper (uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    asm volatile (
        "ecall"
        : [a0] "+r" (a0)
        : "r" (a1), "r" (a2), "r" (a3)
        : "memory"
    );
    return a0;
}

void* mem_alloc (size_t size) {
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    return (int) syscallWrapper (0x02, (uint64) address);
}