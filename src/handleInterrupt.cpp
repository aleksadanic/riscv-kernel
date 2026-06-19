#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Thread.hpp"
#include "../h/Semaphore.hpp"

extern "C" {
    uint64 handleInterrupt (uint64 syscallNum, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {
        printString ("handleInterrupt: ");
        printInt(syscallNum);
        printString (", ");
        uint64 scause_val;
        asm volatile ("csrr %[scause], scause" : [scause] "=r" (scause_val));
        printInt(scause_val);
        printString ("\n");
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
            case 0x21:
                return (uint64) Semaphore::open ((Semaphore**) a1, (unsigned) a2);
            case 0x22:
                return (uint64) Semaphore::close ((Semaphore*) a1);
            case 0x23:
                return (uint64) ((Semaphore*) a1)->wait ();
            case 0x24:
                return (uint64) ((Semaphore*) a1)->signal ();
            case 0x25:
                return (uint64) ((Semaphore*) a1)->wait ((unsigned) a2);
            case 0x26:
                return (uint64) ((Semaphore*) a1)->signal ((unsigned) a2);
        }
        return (uint64) -1;
    }
}