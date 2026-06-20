.global threadStartup
.type threadStartup, @function
threadStartup:
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
    ld a0, 112(tp)
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

    csrw sepc, ra

    csrr t0, sstatus

    li t1, 32
    not t1, t1
    and t0, t0, t1

    li t1, 256
    not t1, t1
    and t0, t0, t1
    
    csrw sstatus, t0

    sret