#include "../lib/hw.h"

struct Context {
    uint64 sp;
    uint64 ra;
    uint64 s[12];
    uint64 a[8];
    uint64 t[7];
    uint64 gp;
    uint64 tp;

    uint64 kernel_sp;

    uint64 sepc;
    uint64 sstatus;
    uint64 scause;

    Context () { }
};

class TCB {
public:
    Context* context;
    TCB* scheduler_next;
    TCB* sem_next;
    TCB* sleep_next;
    int semWaitingForCount;
    int semWaitReturnValue;
    int sleepingMoreFor;

    static TCB* getRunning ();

    static TCB* getIdle ();
    static void setIdle (TCB* t);

    static int create (TCB** handle, void (*start_routine) (void*), void* arg, void* stack_space);
    static int exit ();
    static void dispatch ();
    static int adopt (TCB** handle);
    static void onTickUpdate ();
    static int sleep (time_t time);

    enum class State {
        RUNNING,
        READY,
        BLOCKED,
        FINISHED
    };

    State getState ();
    void setState (State state);

    static void* operator new (size_t size);
    static void* operator new[] (size_t size);
    static void operator delete (void* address);
    static void operator delete[] (void* address);

    ~TCB ();

private:
    uint64* userStack;
    uint64* kernelStack;

    static time_t timeSlice;
    static time_t timeRemaining;

    static TCB* running;
    static TCB* idle;

    static int count;

    State state = State::READY;
};