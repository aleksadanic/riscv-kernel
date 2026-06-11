class Thread;

class Scheduler {
public:
    static void put (Thread* t);
    static Thread* get ();
private:
    static Thread *head, *tail;
};