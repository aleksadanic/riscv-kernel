class SCB;

struct BoundedBuffer {
    static const int N = 256;
    
    char buffer[N];
    int head = 0, tail = 0, size = 0;

    int put (char c);
    char get ();

    bool empty ();
};

class CCB {
public:
    static char getc ();
    static void putc (char c);

    static void handleInterrupt ();
    static void printer (void*);

    static bool outputBufferEmpty ();

    static void init ();

private:
    static BoundedBuffer inputBuffer;
    static BoundedBuffer outputBuffer;

    static SCB* outputData;
    static SCB* outputSpace;
    static SCB* inputData;
};