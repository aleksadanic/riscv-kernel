class TCB;

class Scheduler {
public:
    static void put (TCB* t);
    static TCB* get ();

private:
    static TCB *head, *tail;
};