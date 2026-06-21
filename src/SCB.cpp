#include "../h/SCB.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../h/TCB.hpp"

int SCB::open (SCB** handle, unsigned init) {
    SCB* sem = (SCB*) MemoryAllocator::alloc (sizeof (SCB));
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

int SCB::close (SCB* handle) {
    handle->release ();
    handle->closed = true;
    while (handle->head) {
        TCB* t = handle->get ();
        t->setState (TCB::State::READY);
        t->semWaitReturnValue = -1;
        Scheduler::put (t);
    }
    if (!handle->blockedCount) {
        return MemoryAllocator::free (handle);
    }
    return 0;
}

int SCB::wait () {
    return wait (1);
}

int SCB::signal () {
    return signal (1);
}

int SCB::wait (unsigned n) {
    if (closed) {
        return -1;
    }
    if ((unsigned) val >= n) {
        val -= n;
        return 0;
    }
    TCB* running = TCB::getRunning ();
    running->semWaitingForCount = n;
    running->semWaitReturnValue = 0;
    running->setState (TCB::State::BLOCKED);
    put (running);
    blockedCount++;
    TCB::dispatch ();
    blockedCount--;
    if (closed && !blockedCount) {
        MemoryAllocator::free (this);
    }
    return running->semWaitReturnValue;
}

int SCB::signal (unsigned n) {
    val += n;
    release ();
    return 0;
}

void SCB::release () {
    while (head) {
        if (val >= head->semWaitingForCount) {
            val -= head->semWaitingForCount;
            head->setState (TCB::State::READY);
            Scheduler::put (get ());
            continue;
        }
        TCB* node = head;
        bool done = true;
        while (node->sem_next) {
            if (val >= node->sem_next->semWaitingForCount) {
                val -= node->sem_next->semWaitingForCount;
                node->sem_next->setState (TCB::State::READY);
                TCB* t = node->sem_next;
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

void SCB::put (TCB* t) {
    t->sem_next = 0;
    if (!tail) {
        head = t;
        tail = t;
        return;
    }
    tail->sem_next = t;
    tail = t;
}

TCB* SCB::get () {
    TCB* answer = head;
    head = head->sem_next;
    if (!head) {
        tail = 0;
    }
    return answer;
}