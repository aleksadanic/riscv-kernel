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

    Context () { }
};

class Thread {
public:
    Context* context;
    Thread* scheduler_next;
    Thread* sem_next;
    int semWaitingForCount;

    static Thread* getRunning ();

    static Thread* getIdle ();
    static void setIdle (Thread* t);

    static int create (Thread** handle, void (*start_routine) (void*), void* arg, void* stack_space);
    static int exit ();
    static void dispatch ();
    static int adopt (Thread** handle);

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
    uint64* userStack = 0;
    uint64* kernelStack = 0;

    static Thread* running;
    static Thread* idle;

    static int count;

    State state = State::READY;
};