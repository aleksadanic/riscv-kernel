#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Thread.hpp"
#include "../h/syscall_c.hpp"

extern "C" {
    void interruptRoutine ();

    uint64 handleInterrupt (uint64 syscallNum, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {
        printString("handleInterrupt\n");
        printInt(syscallNum);
        __putc('\n');
        switch (syscallNum) {
            case 0x01:
                return (uint64) MemoryAllocator::alloc ((size_t) a1);
            case 0x02:
                return (uint64) MemoryAllocator::free ((void*) a1);
            case 0x11:
                return (uint64) Thread::create ((Thread**) a1, (void(*)(void*)) a2, (void*) a3, (void*) a4);
            case 0x12:
                return (uint64) Thread::exit ();
            case 0x13:
                Thread::dispatch ();
                return (uint64) 0;
        }
        return (uint64) -1;
    }
}

void* operator new (size_t size) {
    return mem_alloc(size);
}

void* operator new[] (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* address) {
    mem_free(address);
}

void operator delete[] (void* address) {
    mem_free(address);
}

void main () {
    printString("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    printString("Interrupt routine address initialized!\n");
}