#include "../h/SleepQueue.hpp"
#include "../h/Thread.hpp"
#include "../h/Scheduler.hpp"

void SleepQueue::put (time_t time) {
    Thread* running = Thread::getRunning ();
    if (!head) {
        head = running;
        running->sleepingMoreFor = time;
        running->sleep_next = 0;
    } else if ((time_t) head->sleepingMoreFor >= time) {
        running->sleep_next = head;
        head->sleepingMoreFor -= time;
        running->sleepingMoreFor = time;
        head = running;
    } else {
        Thread* curr = head;
        time_t total = 0;
        total = curr->sleepingMoreFor;
        while (curr->sleep_next) {
            if (total + curr->sleep_next->sleepingMoreFor >= time) {
                break;
            }
            curr = curr->sleep_next;
            total += curr->sleepingMoreFor;
        }
        running->sleep_next = curr->sleep_next;
        curr->sleep_next = running;
        running->sleepingMoreFor = time - total;
        if (running->sleep_next) {
            running->sleep_next->sleepingMoreFor -= time - total;
        }
    }
    running->setState (Thread::State::BLOCKED);
    Thread::dispatch ();
}

Thread* SleepQueue::get () {
    Thread* answer = head;
    head = head->sleep_next;
    return answer;
}

void SleepQueue::forward (time_t time) {
    while (head) {
        if ((time_t) head->sleepingMoreFor > time) {
            head->sleepingMoreFor -= time;
            break;
        }
        time -= head->sleepingMoreFor;
        Thread* t = get ();
        t->setState (Thread::State::READY);
        Scheduler::put (t);
    }
}

Thread* SleepQueue::head = 0;