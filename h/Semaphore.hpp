class Thread;

class Semaphore {
public:
    static int open (Semaphore** handle, unsigned init);
    static int close (Semaphore* handle);

    int wait ();
    int signal ();

    int wait (unsigned n);
    int signal (unsigned n);

private:
    int val;

    int blockedCount;
    bool closed;

    Thread *head, *tail;

    void release ();

    void put (Thread* t);
    Thread* get ();
};