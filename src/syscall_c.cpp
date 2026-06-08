#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

void* mem_alloc (size_t size) {
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    void* address;
    asm volatile (
        "li a0, 0x1\n"
        "mv a1, %[blocks]\n"
        "ecall\n"
        "mv %[address], a0"
        : [address] "=r" (address)
        : [blocks] "r" (blocks)
        : "a0", "a1"
    );
    return address;
}