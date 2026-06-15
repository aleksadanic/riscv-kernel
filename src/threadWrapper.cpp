#include "../lib/console.h"
#include "../h/syscall_c.hpp"

void threadWrapper (void (*start_routine) (void*), void* arg) {
    printString ("threadWrapper\n");
    start_routine (arg);
    thread_exit ();
}