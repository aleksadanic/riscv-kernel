#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/CCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain ();
void idle (void* arg);

void userMainWrapper (void*) {
    userMain ();
}

void main () {
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    MemoryAllocator::init ();
    TCB* mainTCB;
    if (TCB::adopt (&mainTCB)) {
        return;
    }
    thread_t idleTCB;
    if (thread_create (&idleTCB, &idle, nullptr)) {
        mem_free (mainTCB);
        return;
    }
    TCB::setIdle ((TCB*) idleTCB);
    Scheduler::get ();
    thread_t printerTCB;
    if (thread_create (&printerTCB, &CCB::printer, nullptr)) {
        mem_free (mainTCB);
        mem_free (idleTCB);
        return;
    }
    ((TCB*) printerTCB)->context->sstatus = 256;
    thread_t userMainTCB;
    if (thread_create (&userMainTCB, &userMainWrapper, nullptr)) {
        mem_free (mainTCB);
        mem_free (idleTCB);
        mem_free (printerTCB);
        return;
    }
    CCB::init ();
    asm volatile (
        "li t0, 514\n"
        "csrw sie, t0\n"
        "csrw sip, zero\n"
        : : : "t0"
    );
    thread_exit ();
}