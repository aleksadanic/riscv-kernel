#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/Thread.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain (void* arg);
void idle (void* arg);

void main () {
    // printString ("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    // printString ("Interrupt routine address initialized!\n");
    Thread* mainThread;
    if (Thread::adopt (&mainThread)) {
        // printString ("FATAL ERROR: mainThread not initialized\n");
        return;
    }
    // printString ("mainThread initialized!\n");
    Thread* idleThread;
    if (thread_create (&idleThread, &idle, nullptr)) {
        mem_free (mainThread);
        // printString ("FATAL ERROR: idleThread not initialized\n");
        return;
    }
    // printString ("idleThread created!\n");
    Thread::setIdle (idleThread);
    Scheduler::get ();
    Thread* userMainThread;
    if (thread_create (&userMainThread, &userMain, nullptr)) {
        mem_free (mainThread);
        mem_free (idleThread);
        // printString ("FATAL ERROR: userMainThread not initialized\n");
        return;
    }
    // printString ("userMainThread created!\n");
    asm volatile (
        "li t0, 2\n"
        "csrw sie, t0\n"
        "csrw sip, zero\n"
        : : : "t0"
    );
    thread_exit ();
}