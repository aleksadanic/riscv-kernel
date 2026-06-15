#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/Thread.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain (void* arg);

void main () {
    printString ("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    printString ("Interrupt routine address initialized!\n");
    Thread* mainThread;
    if (Thread::adopt (&mainThread)) {
        printString ("FATAL ERROR: mainThread not initialized\n");
        return;
    }
    // asm volatile (
    //     "csrr t0, sstatus\n"
    //     "li t1, 256\n"
    //     "not t1, t1\n"
    //     "and t0, t0, t1\n"
    //     "csrw sstatus, t0\n"
    //     : : : "t0", "t1"
    // );
    printString ("mainThread initialized!\n");
    Thread* userMainThread;
    if (thread_create (&userMainThread, &userMain, nullptr)) {
        mem_free (mainThread);
        printString ("FATAL ERROR: userMainThread not initialized\n");
        return;
    }
    // asm volatile (
    //     "csrr t0, sepc" : : : "t0"
    // );
    printString ("userMainThread created!\n");
    thread_exit ();
}