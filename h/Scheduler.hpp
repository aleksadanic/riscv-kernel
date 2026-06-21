class TCB;

class Scheduler {
public:
    static void put (TCB* t);
    static TCB* get ();

    static void operator delete (void* address);

private:
    static TCB *head, *tail;
};