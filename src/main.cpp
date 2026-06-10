#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

extern "C" {
    void interruptRoutine ();

    uint64 handleInterrupt (uint64 syscallNum, uint64 a1, uint64 a2, uint64 a3) {
        printString("handleInterrupt\n");
        printInt(syscallNum);
        __putc('\n');
        switch (syscallNum) {
            case 0x01:
                return (uint64) MemoryAllocator::alloc((size_t) a1);
            case 0x02:
                return (uint64) MemoryAllocator::free((void*) a1);
        }
        return (uint64) -1;
    }
}

void main () {
    printString("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    printString("Interrupt routine address initialized!\n");
}