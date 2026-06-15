#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/Thread.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain (void* arg);

void main () {
    printInt ((uint64) &userMain);
    printString("\n");
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