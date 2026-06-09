#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

uint64 syscallWrapper (uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    uint64 answer;
    asm volatile (
        "ecall\n"
        "mv %[answer], a0"
        : [answer] "=r" (answer)
        : : "a0", "memory"
    );
    return answer;
}

void* mem_alloc (size_t size) {
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*) syscallWrapper (0x01, blocks);
}

int mem_free (void* address) {
    return (int) syscallWrapper (0x02, (uint64) address);
}