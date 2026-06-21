class TCB;
typedef unsigned long time_t;

class SleepQueue {
public:
    static void put (time_t time);
    static TCB* get ();
    static void forward (time_t time);

private:
    static TCB* head;
};