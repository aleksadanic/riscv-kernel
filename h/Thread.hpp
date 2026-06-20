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

    Context () { }
};

class Thread {
public:
    Context* context;
    Thread* scheduler_next;
    Thread* sem_next;
    Thread* sleep_next;
    int semWaitingForCount;
    int semWaitReturnValue;
    int sleepingMoreFor;

    static Thread* getRunning ();

    static Thread* getIdle ();
    static void setIdle (Thread* t);

    static int create (Thread** handle, void (*start_routine) (void*), void* arg, void* stack_space);
    static int exit ();
    static void dispatch ();
    static int adopt (Thread** handle);
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

    ~Thread ();

private:
    uint64* userStack;
    uint64* kernelStack;

    time_t timeSlice = DEFAULT_TIME_SLICE * 1000;
    time_t timeRemaining = DEFAULT_TIME_SLICE * 1000;

    static Thread* running;
    static Thread* idle;

    static int count;

    State state = State::READY;
};