#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

extern "C" {
    void interruptRoutine ();

    uint64 handleInterrupt (uint64 syscallNum, uint64 a1, uint64 a2, uint64 a3) {
        switch (syscallNum) {
            case 0x01:
                return (uint64) MemoryAllocator::getInstance().alloc((size_t) a1);
            case 0x02:
                return (uint64) MemoryAllocator::getInstance().free((void*) a1);
        }
        return (uint64) -1;
    }
}

void main () {
    __putc('h');
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    __putc('s');
    mem_alloc(120);
    __putc('e');
}