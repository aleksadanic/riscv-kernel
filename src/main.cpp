#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/TCB.hpp"
#include "../h/CCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"

extern "C" void interruptRoutine ();

void userMain (void* arg);
void idle (void* arg);

void main () {
    // printString ("main\n");
    asm volatile ("csrw stvec, %[interruptRoutine]" : : [interruptRoutine] "r" (&interruptRoutine));
    // printString ("Interrupt routine address initialized!\n");
    TCB* mainTCB;
    if (TCB::adopt (&mainTCB)) {
        // printString ("FATAL ERROR: mainTCB not initialized\n");
        return;
    }
    // printString ("mainTCB initialized!\n");
    TCB* idleTCB;
    if (thread_create (&idleTCB, &idle, nullptr)) {
        mem_free (mainTCB);
        // printString ("FATAL ERROR: idleTCB not initialized\n");
        return;
    }
    // printString ("idleTCB created!\n");
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
        // printString ("FATAL ERROR: userMainTCB not initialized\n");
        return;
    }
    // printString ("userMainTCB created!\n");
    CCB::init ();
    asm volatile (
        "li t0, 514\n"
        "csrw sie, t0\n"
        "csrw sip, zero\n"
        : : : "t0"
    );
    thread_exit ();
}