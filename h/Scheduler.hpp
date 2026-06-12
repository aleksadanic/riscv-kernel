class Thread;

class Scheduler {
public:
    static void put (Thread* t);
    static Thread* get ();

    static void operator delete (void* address);

private:
    static Thread *head, *tail;
};