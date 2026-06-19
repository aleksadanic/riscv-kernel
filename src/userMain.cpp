#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void f (void* arg) {

}

void userMain (void* arg) {
    printString ("userMain\n");
    Semaphore* sem;
    sem_open (&sem, 1);
    sem_wait_n (sem, 2);
    printString("Rispect\n");
    sem_signal (sem);
    sem_close (sem);
    // Thread* F;
    // thread_create (&F, &f, nullptr);
}