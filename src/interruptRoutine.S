.global interruptRoutine
.type interruptRoutine, @function
interruptRoutine:
    csrrw tp, sscratch, tp

    sd sp, 0(tp)
    sd ra, 8(tp)
    sd s0, 16(tp)
    sd s1, 24(tp)
    sd s2, 32(tp)
    sd s3, 40(tp)
    sd s4, 48(tp)
    sd s5, 56(tp)
    sd s6, 64(tp)
    sd s7, 72(tp)
    sd s8, 80(tp)
    sd s9, 88(tp)
    sd s10, 96(tp)
    sd s11, 104(tp)
    sd a0, 112(tp)
    sd a1, 120(tp)
    sd a2, 128(tp)
    sd a3, 136(tp)
    sd a4, 144(tp)
    sd a5, 152(tp)
    sd a6, 160(tp)
    sd a7, 168(tp)
    sd t0, 176(tp)
    sd t1, 184(tp)
    sd t2, 192(tp)
    sd t3, 200(tp)
    sd t4, 208(tp)
    sd t5, 216(tp)
    sd t6, 224(tp)
    sd gp, 232(tp)

    csrr gp, sscratch
    sd gp, 240(tp)

    ld sp, 248(tp)

    csrw sscratch, tp

    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0

    call handleInterrupt

    csrr tp, sscratch

    sd sp, 248(tp)

    ld sp, 0(tp)
    ld ra, 8(tp)
    ld s0, 16(tp)
    ld s1, 24(tp)
    ld s2, 32(tp)
    ld s3, 40(tp)
    ld s4, 48(tp)
    ld s5, 56(tp)
    ld s6, 64(tp)
    ld s7, 72(tp)
    ld s8, 80(tp)
    ld s9, 88(tp)
    ld s10, 96(tp)
    ld s11, 104(tp)
    ld a1, 120(tp)
    ld a2, 128(tp)
    ld a3, 136(tp)
    ld a4, 144(tp)
    ld a5, 152(tp)
    ld a6, 160(tp)
    ld a7, 168(tp)
    ld t0, 176(tp)
    ld t1, 184(tp)
    ld t2, 192(tp)
    ld t3, 200(tp)
    ld t4, 208(tp)
    ld t5, 216(tp)
    ld t6, 224(tp)
    ld gp, 232(tp)
    ld tp, 240(tp)

    sret