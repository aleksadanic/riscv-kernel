#include "../h/Thread.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"

#define KERNEL_STACK_SIZE 512

extern "C" {
    void contextSwitch (Context* c1, Context* c2);
    void threadStartup ();
};

int Thread::create (Thread** handle, void(*start_routine)(void*), void* arg, void* stack_space) {
    if (!start_routine) {
        return -2;
    }
    uint64* userStack = (uint64*) ((char*) stack_space - DEFAULT_STACK_SIZE);
    uint64* kernelStack = new uint64[KERNEL_STACK_SIZE];
    Context* context = new Context;
    Thread* t = new Thread;
    if (!kernelStack || !context || !t) {
        delete[] userStack;
        if (kernelStack) {
            delete[] kernelStack;
        }
        if (context) {
            delete context;
        }
        if (t) {
            delete t;
        }
        return -1;
    }
    context->sp = (uint64) stack_space;
    context->ra = (uint64) &threadWrapper;
    context->a[0] = (uint64) start_routine;
    context->a[1] = (uint64) arg;
    context->kernel_sp = ((uint64) (kernelStack + KERNEL_STACK_SIZE) + 15) / 16 * 16 - 112;
    *((uint64*) context->kernel_sp) = (uint64) &threadStartup;
    t->userStack = userStack;
    t->kernelStack = kernelStack;
    t->context = context;
    *handle = t;
    return 0;
}

int Thread::exit () {
    running->state = State::FINISHED;
    Thread* oldRunning = running;
    running = Scheduler::get ();
    contextSwitch (oldRunning->context, running->context);
    return 0;
}

void Thread::dispatch () {
    Thread* oldRunning = running;
    Scheduler::put (running);
    running = Scheduler::get ();
    if (oldRunning != running) {
        contextSwitch(oldRunning->context, running->context);
    }
}

void Thread::threadWrapper (void (*start_routine) (void*), void* arg) {
    start_routine (arg);
    exit ();
}

Thread::State Thread::getState () {
    return state;
}

Thread::~Thread () {
    delete[] userStack;
    delete[] kernelStack;
    delete context;
}

Thread* Thread::running = 0;

void* Thread::operator new (size_t size) {
    return MemoryAllocator::alloc ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void* Thread::operator new[] (size_t size) {
    return MemoryAllocator::alloc ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void Thread::operator delete (void* address) {
    MemoryAllocator::free (address);
}

void Thread::operator delete[] (void* address) {
    MemoryAllocator::free (address);
}