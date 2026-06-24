#include "../h/syscall_c.hpp"

void threadWrapper (void (*start_routine) (void*), void* arg) {
    start_routine (arg);
    thread_exit ();
}