#include "../h/Scheduler.hpp"
#include "../h/Thread.hpp"
#include "../h/MemoryAllocator.hpp"

void Scheduler::put (Thread* t) {
    if (!t) {
        return;
    }
    t->next = 0;
    if (!tail) {
        head = t;
        tail = t;
        return;
    }
    tail->next = t;
    tail = t;
}

Thread* Scheduler::get () {
    while (true) {
        Thread *answer = head;
        if (head) {
            head = head->next;
            if (!head) {
                tail = 0;
            }
        }
        if (answer && answer->getState() == Thread::State::FINISHED) {
            delete answer;
            continue;
        }
        return answer;
    }
}

Thread* Scheduler::head = 0;
Thread* Scheduler::tail = 0;

void Scheduler::operator delete (void* address) {
    MemoryAllocator::free (address);
}