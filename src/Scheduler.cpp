#include "../h/Scheduler.hpp"
#include "../h/Thread.hpp"

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
    Thread* answer = head;
    if (head) {
        head = head->next;
        if (!head) {
            tail = 0;
        }
    }
    return answer;
}

Thread* Scheduler::head = 0;
Thread* Scheduler::tail = 0;