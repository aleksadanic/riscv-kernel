#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/SCB.hpp"
#include "../h/CCB.hpp"

void exitProgram ();

extern "C" {
    uint64 handleInterrupt (uint64 syscallNum, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {
        uint64 scause;
        asm volatile ("csrr %[scause], scause" : [scause] "=r" (scause));
        // PrintString ("handleInterrupt: ");
        // PrintInt(syscallNum);
        // PrintString (", ");
        // PrintInt(scause);
        // PrintString ("\n");
        if (scause == 2) {
            PrintString ("FATAL ERROR: Illegal Instruction\n");
            exitProgram ();
        }
        if (scause == 0x8000000000000001) {
            asm volatile (
                "csrr t0, sip\n"
                "li t1, 2\n"
                "not t1, t1\n"
                "and t0, t0, t1\n"
                "csrw sip, t0"
                : : : "t0", "t1"
            );
            TCB::onTickUpdate ();
            return (uint64) 0;
        }
        if (scause == 0x8000000000000009) {
            int irq = plic_claim();
            if (irq == CONSOLE_IRQ) {
                CCB::handleInterrupt();
            }
            plic_complete(irq);
            return (uint64) 0;
        }
        TCB::getRunning ()->context->sepc += 4;
        switch (syscallNum) {
            case 0x01:
                return (uint64) MemoryAllocator::alloc ((size_t) a1);
            case 0x02:
                return (uint64) MemoryAllocator::free ((void*) a1);
            case 0x11:
                return (uint64) TCB::create ((TCB**) a1, (void(*)(void*)) a2, (void*) a3, (void*) a4);
            case 0x12:
                return (uint64) TCB::exit ();
            case 0x13:
                TCB::dispatch ();
                return (uint64) 0;
            case 0x21:
                return (uint64) SCB::open ((SCB**) a1, (unsigned) a2);
            case 0x22:
                return (uint64) SCB::close ((SCB*) a1);
            case 0x23:
                return (uint64) ((SCB*) a1)->wait ();
            case 0x24:
                return (uint64) ((SCB*) a1)->signal ();
            case 0x25:
                return (uint64) ((SCB*) a1)->wait ((unsigned) a2);
            case 0x26:
                return (uint64) ((SCB*) a1)->signal ((unsigned) a2);
            case 0x31:
                return (uint64) TCB::sleep ((time_t) a1);
            case 0x41:
                return (uint64) CCB::getc ();
            case 0x42:
                CCB::putc ((char) a1);
                return (uint64) 0;
        }
        return (uint64) -1;
    }
}