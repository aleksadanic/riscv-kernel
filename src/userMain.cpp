#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void worker(void* arg) {
    int id = (uint64)arg;

    for (int i = 0; i < 20; i++) {
        printString("T");
        printInt(id);
        printString(" i=");
        printInt(i);
        printString("\n");

        if (i % 3 == 0) {
            thread_dispatch();
        }

        for (volatile int j = 0; j < 10000000; j++);
    }
}

void userMain(void*) {
    Thread* t1;
    Thread* t2;

    thread_create(&t1, worker, (void*)1);
    thread_create(&t2, worker, (void*)2);

    for (volatile int i = 0; i < 500000000; i++);
}