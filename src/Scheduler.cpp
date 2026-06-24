#include "../h/Scheduler.hpp"
#include "../h/TCB.hpp"
#include "../h/MemoryAllocator.hpp"

void Scheduler::put (TCB* t) {
    if (!t) {
        return;
    }
    t->scheduler_next = 0;
    if (!tail) {
        head = t;
        tail = t;
        return;
    }
    tail->scheduler_next = t;
    tail = t;
}

TCB* Scheduler::get () {
    while (true) {
        TCB *answer = head;
        if (head) {
            head = head->scheduler_next;
            if (!head) {
                tail = 0;
            }
        }
        if (answer && answer->getState() == TCB::State::FINISHED) {
            delete answer;
            continue;
        }
        if (!answer) {
            return TCB::getIdle ();
        }
        return answer;
    }
}

TCB* Scheduler::head = 0;
TCB* Scheduler::tail = 0;