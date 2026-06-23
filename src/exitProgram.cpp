void exitProgram () {
    asm volatile (
        "li t0, 0x100000\n"
        "li t1, 0x5555\n"
        "sw t1, 0(t0)\n"
        : : : "t0", "t1", "memory"
    );
}