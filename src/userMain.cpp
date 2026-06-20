#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void A(void*) {
    for (int i = 0; i < 5; i++) {
        printString("A ");
        printInt(i);
        printString("\n");
        time_sleep(3);
    }
}

void B(void*) {
    for (int i = 0; i < 10; i++) {
        printString("B ");
        printInt(i);
        printString("\n");
        thread_dispatch();
    }
}

void userMain(void*) {
    Thread* a;
    Thread* b;

    thread_create(&a, A, nullptr);
    thread_create(&b, B, nullptr);

    for (volatile int i = 0; i < 300000000; i++);
}