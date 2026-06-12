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
    Thread* next;

    static Thread* running;

    static int create (Thread** handle, void (*start_routine) (void*), void* arg, void* stack_space);
    static int exit ();
    static void dispatch ();

    enum class State {
        READY,
        FINISHED
    };

    State getState ();

    static void* operator new (size_t size);
    static void* operator new[] (size_t size);
    static void operator delete (void* address);
    static void operator delete[] (void* address);

    ~Thread ();

private:
    uint64* userStack;
    uint64* kernelStack;

    State state = State::READY;

    static void threadWrapper (void (*start_routine) (void*), void* arg);
};