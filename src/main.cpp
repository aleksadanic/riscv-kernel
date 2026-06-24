#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/CCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain (void* arg);
void idle (void* arg);

void main () {
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    TCB* mainTCB;
    if (TCB::adopt (&mainTCB)) {
        return;
    }
    TCB* idleTCB;
    if (thread_create (&idleTCB, &idle, nullptr)) {
        mem_free (mainTCB);
        return;
    }
    TCB::setIdle (idleTCB);
    Scheduler::get ();
    TCB* printerTCB;
    if (thread_create (&printerTCB, &CCB::printer, nullptr)) {
        mem_free (mainTCB);
        mem_free (idleTCB);
        return;
    }
    printerTCB->context->sstatus = 256;
    TCB* userMainTCB;
    if (thread_create (&userMainTCB, &userMain, nullptr)) {
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