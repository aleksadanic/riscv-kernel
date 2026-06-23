#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* address) {
    mem_free (address);
}

Thread::Thread (void (*body) (void*), void* arg) : body (body), arg (arg) { }
Thread::~Thread () { }
int Thread::start () { return thread_create (&myHandle, body, arg); }
void Thread::dispatch () { thread_dispatch (); }
int Thread::sleep (time_t time) { return time_sleep (time); }
Thread::Thread () : body (&runWrapper), arg (this) { }
void Thread::runWrapper (void* arg) { ((Thread*) arg)->run (); }

Semaphore::Semaphore (unsigned init) { sem_open (&myHandle, init); }
Semaphore::~Semaphore () { sem_close (myHandle); }
int Semaphore::wait () { return sem_wait (myHandle); }
int Semaphore::signal () { return sem_signal (myHandle); }

void PeriodicThread::terminate () { period = 0; }
PeriodicThread::PeriodicThread (time_t period) : period (period) { }
void PeriodicThread::run () {
    while (period) {
        periodicActivation ();
        time_sleep (period);
    }
}

char Console::getc () {
    return ::getc ();
}

void Console::putc (char c) {
    ::putc (c);
}