#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

extern "C" {
    void contextSwitch (Context* a1, Context* a2);
    void interruptRoutine ();

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
            case 0x14:
                return (uint64) Thread::adopt ((Thread**) a1);
        }
        return (uint64) -1;
    }
}

void threadWrapper (void (*start_routine) (void*), void* arg) {
    printString ("threadWrapper\n");
    start_routine (arg);
    thread_exit ();
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

void userMain (void* ptr) {
    printString ("userMain\n");
    Thread *F, *G, *H;
    thread_create (&F, &f, nullptr);
    thread_create (&G, &g, nullptr);
    thread_create (&H, &h, nullptr);
    thread_exit ();
}

void main () {
    printString ("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    printString ("Interrupt routine address initialized!\n");
    Thread* mainThread;
    if (Thread::adopt (&mainThread)) {
        printString ("FATAL ERROR: mainThread not initialized\n");
        return;
    }
    printString ("mainThread initialized!\n");
    Thread* userMainThread;
    if (thread_create (&userMainThread, &userMain, nullptr)) {
        mem_free (mainThread);
        printString ("FATAL ERROR: userMainThread not initialized\n");
        return;
    }
    printString ("userMainThread created!\n");
    thread_exit ();
}