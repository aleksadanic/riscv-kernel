#include "../lib/console.h"

void PrintString(const char* s) {
    while (*s) {
        __putc(*s++);
    }
}

void PrintInt(long x) {
    if (x == 0) {
        __putc('0');
        return;
    }

    if (x < 0) {
        __putc('-');
        x = -x;
    }

    char buf[20];
    int i = 0;

    while (x > 0) {
        buf[i++] = '0' + x % 10;
        x /= 10;
    }

    while (i > 0) {
        __putc(buf[--i]);
    }
}