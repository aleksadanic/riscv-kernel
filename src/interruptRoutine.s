.global interruptRoutine
.type interruptRoutine, @function
interruptRoutine:
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    call handleInterrupt
    sret