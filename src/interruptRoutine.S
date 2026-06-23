.global interruptRoutine
.type interruptRoutine, @function
interruptRoutine:
    csrrw t0, sscratch, t0

    sd sp, 0(t0)
    sd ra, 8(t0)
    sd s0, 16(t0)
    sd s1, 24(t0)
    sd s2, 32(t0)
    sd s3, 40(t0)
    sd s4, 48(t0)
    sd s5, 56(t0)
    sd s6, 64(t0)
    sd s7, 72(t0)
    sd s8, 80(t0)
    sd s9, 88(t0)
    sd s10, 96(t0)
    sd s11, 104(t0)
    sd a0, 112(t0)
    sd a1, 120(t0)
    sd a2, 128(t0)
    sd a3, 136(t0)
    sd a4, 144(t0)
    sd a5, 152(t0)
    sd a6, 160(t0)
    sd a7, 168(t0)

    sd t1, 184(t0)
    sd t2, 192(t0)
    sd t3, 200(t0)
    sd t4, 208(t0)
    sd t5, 216(t0)
    sd t6, 224(t0)
    sd gp, 232(t0)
    sd tp, 240(t0)

    csrr t1, sscratch
    sd t1, 176(t0)

    csrr t1, sepc
    sd t1, 256(t0)

    csrr t1, sstatus
    sd t1, 264(t0)

    csrr t1, scause
    sd t1, 272(t0)

    ld sp, 248(t0)

    csrw sscratch, t0

    li tp, 0

    call handleInterrupt

    csrr t0, sscratch

    sd sp, 248(t0)

    ld t1, 256(t0)
    csrw sepc, t1

    ld t1, 264(t0)
    csrw sstatus, t1

    ld sp, 0(t0)
    ld ra, 8(t0)
    ld s0, 16(t0)
    ld s1, 24(t0)
    ld s2, 32(t0)
    ld s3, 40(t0)
    ld s4, 48(t0)
    ld s5, 56(t0)
    ld s6, 64(t0)
    ld s7, 72(t0)
    ld s8, 80(t0)
    ld s9, 88(t0)
    ld s10, 96(t0)
    ld s11, 104(t0)

    ld t1, 272(t0)

    li t2, 8
    beq t2, t1, continue

    li t2, 9
    beq t2, t1, continue

    ld a0, 112(t0)

continue:
    ld a1, 120(t0)
    ld a2, 128(t0)
    ld a3, 136(t0)
    ld a4, 144(t0)
    ld a5, 152(t0)
    ld a6, 160(t0)
    ld a7, 168(t0)

    ld t1, 184(t0)
    ld t2, 192(t0)
    ld t3, 200(t0)
    ld t4, 208(t0)
    ld t5, 216(t0)
    ld t6, 224(t0)
    ld gp, 232(t0)
    ld tp, 240(t0)

    ld t0, 176(t0)

    sret