#include "../h/Semaphore.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../h/Thread.hpp"

int Semaphore::open (Semaphore** handle, unsigned init) {
    Semaphore* sem = (Semaphore*) MemoryAllocator::alloc (sizeof (Semaphore));
    if (!sem) {
        return -1;
    }
    sem->val = init;
    sem->blockedCount = 0;
    sem->closed = false;
    sem->head = 0;
    sem->tail = 0;
    *handle = sem;
    return 0;
}

int Semaphore::close (Semaphore* handle) {
    handle->release ();
    handle->closed = true;
    while (handle->head) {
        Thread* t = handle->get ();
        t->setState (Thread::State::READY);
        t->semWaitReturnValue = -1;
        Scheduler::put (t);
    }
    if (!handle->blockedCount) {
        return MemoryAllocator::free (handle);
    }
    return 0;
}

int Semaphore::wait () {
    return wait (1);
}

int Semaphore::signal () {
    return signal (1);
}

int Semaphore::wait (unsigned n) {
    if (closed) {
        return -1;
    }
    if ((unsigned) val >= n) {
        val -= n;
        return 0;
    }
    Thread* running = Thread::getRunning ();
    running->semWaitingForCount = n;
    running->semWaitReturnValue = 0;
    running->setState (Thread::State::BLOCKED);
    put (running);
    blockedCount++;
    Thread::dispatch ();
    blockedCount--;
    if (closed && !blockedCount) {
        MemoryAllocator::free (this);
    }
    return running->semWaitReturnValue;
}

int Semaphore::signal (unsigned n) {
    val += n;
    release ();
    return 0;
}

void Semaphore::release () {
    while (head) {
        if (val >= head->semWaitingForCount) {
            val -= head->semWaitingForCount;
            head->setState (Thread::State::READY);
            Scheduler::put (get ());
            continue;
        }
        Thread* node = head;
        bool done = true;
        while (node->sem_next) {
            if (val >= node->sem_next->semWaitingForCount) {
                val -= node->sem_next->semWaitingForCount;
                node->sem_next->setState (Thread::State::READY);
                Thread* t = node->sem_next;
                node->sem_next = node->sem_next->sem_next;
                if (tail == t) {
                    tail = node;
                }
                Scheduler::put (t);
                done = false;
                break;
            }
            node = node->sem_next;
        }
        if (done) {
            break;
        }
    }
}

void Semaphore::put (Thread* t) {
    t->sem_next = 0;
    if (!tail) {
        head = t;
        tail = t;
        return;
    }
    tail->sem_next = t;
    tail = t;
}

Thread* Semaphore::get () {
    Thread* answer = head;
    head = head->sem_next;
    if (!head) {
        tail = 0;
    }
    return answer;
}