#include "../h/TCB.hpp"
#include "../h/CCB.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../h/SleepQueue.hpp"

#define KERNEL_STACK_SIZE 4096

void exitProgram ();

extern "C" {
    void contextSwitch (Context* c1, Context* c2);
    void threadStartup ();
};

void threadWrapper (void (*start_routine) (void*), void* arg);

int TCB::create (TCB** handle, void(*start_routine)(void*), void* arg, void* stack_space) {
    if (!start_routine) {
        return -2;
    }
    uint64* userStack = (uint64*) ((char*) stack_space - DEFAULT_STACK_SIZE);
    uint64* kernelStack = (uint64*) MemoryAllocator::alloc ((KERNEL_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    Context* context = new Context ();
    TCB* t = new TCB ();
    if (!kernelStack || !context || !t) {
        if (userStack) {
            MemoryAllocator::free (userStack);
        }
        if (kernelStack) {
            MemoryAllocator::free (kernelStack);
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
    context->kernel_sp = ((uint64) ((char*) kernelStack + KERNEL_STACK_SIZE) + 15) / 16 * 16 - 112;
    context->sstatus = 0;
    *((uint64*) context->kernel_sp) = (uint64) &threadStartup;
    t->userStack = userStack;
    t->kernelStack = kernelStack;
    t->context = context;
    t->state = TCB::State::READY;
    Scheduler::put (t);
    *handle = t;
    count++;
    return 0;
}

int TCB::exit () {
    count--;
    if (!count) {
        while (!CCB::outputBufferEmpty ()) {
            TCB::dispatch ();
        }
        exitProgram ();
    }
    running->state = State::FINISHED;
    TCB* oldRunning = running;
    Scheduler::put (running);
    running = Scheduler::get ();
    running->timeRemaining = running->timeSlice;
    running->state = State::RUNNING;
    contextSwitch (oldRunning->context, running->context);
    return 0;
}

void TCB::dispatch () {
    if (running->getState () == State::RUNNING) {
        running->state = State::READY;
        if (running != idle) {
            Scheduler::put (running);
        }
    }
    TCB* oldRunning = running;
    running = Scheduler::get ();
    running->timeRemaining = running->timeSlice;
    running->state = State::RUNNING;
    if (oldRunning != running) {
        contextSwitch (oldRunning->context, running->context);
    }
}

int TCB::adopt (TCB** handle) {
    uint64* kernelStack = (uint64*) MemoryAllocator::alloc ((KERNEL_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    Context* context = new Context ();
    TCB* t = new TCB ();
    if (!kernelStack || !context || !t) {
        if (kernelStack) {
            MemoryAllocator::free (kernelStack);
        }
        if (context) {
            delete context;
        }
        if (t) {
            delete t;
        }
        return -1;
    }
    context->kernel_sp = ((uint64) (kernelStack + KERNEL_STACK_SIZE) + 15) / 16 * 16 - 112;
    t->kernelStack = kernelStack;
    asm volatile ("csrw sscratch, %[context]" : : [context] "r" (context));
    t->context = context;
    t->state = TCB::State::RUNNING;
    running = t;
    *handle = t;
    count++;
    return 0;
}

void TCB::onTickUpdate () {
    SleepQueue::forward (1);
    TCB* running = getRunning ();
    if (--running->timeRemaining == 0) {
        dispatch ();
    }
}

int TCB::sleep (time_t time) {
    if (!time) {
        return 0;
    }
    SleepQueue::put (time);
    return 0;
}

TCB::State TCB::getState () {
    return state;
}

void TCB::setState (State state) {
    this->state = state;
}

TCB::~TCB () {
    if (userStack) {
        MemoryAllocator::free (userStack);
    }
    if (kernelStack) {
        MemoryAllocator::free (kernelStack);
    }
    if (context) {
        delete context;
    }
}

time_t TCB::timeRemaining = DEFAULT_TIME_SLICE;
time_t TCB::timeSlice = DEFAULT_TIME_SLICE;

TCB* TCB::running = 0;

TCB* TCB::getRunning () {
    return running;
}

TCB* TCB::idle = 0;

TCB* TCB::getIdle () {
    return idle;
}

void TCB::setIdle (TCB* t) {
    idle = t;
}

int TCB::count = -2;

void* TCB::operator new (size_t size) {
    return MemoryAllocator::alloc ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void TCB::operator delete (void* address) {
    MemoryAllocator::free (address);
}

void* Context::operator new (size_t size) {
    return MemoryAllocator::alloc ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void Context::operator delete (void* address) {
    MemoryAllocator::free (address);
}