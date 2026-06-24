.global threadStartup
.type threadStartup, @function
threadStartup:
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
    ld a0, 112(t0)
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

    ld t0, 176(t0)

    sret