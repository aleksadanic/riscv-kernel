class Thread;
typedef unsigned long time_t;

class SleepQueue {
public:
    static void put (time_t time);
    static Thread* get ();
    static void forward (time_t time);

private:
    static Thread* head;
};