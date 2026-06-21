class TCB;

class SCB {
public:
    static int open (SCB** handle, unsigned init);
    static int close (SCB* handle);

    int wait ();
    int signal ();

    int wait (unsigned n);
    int signal (unsigned n);

private:
    int val;

    int blockedCount;
    bool closed;

    TCB *head, *tail;

    void release ();

    void put (TCB* t);
    TCB* get ();
};