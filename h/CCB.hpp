struct Buffer {
    static const int N = 256;

    char buffer[N];
    int head = 0, tail = 0, size = 0;

    int put (char c) {
        if (size == N) {
            return -1;
        }
        buffer[tail] = c;
        tail = (tail + 1) % N;
        size++;
        return 0;
    }

    char get () {
        char c = buffer[head];
        head = (head + 1) % N;
        size--;
        return c;
    }
};

class CCB {
public:
    static char getc ();
    static void putc (char c);

    static void handleInterrupt ();
    static void printer (void*);

    static void init ();

private:
    static Buffer inputBuffer;
    static Buffer outputBuffer;

    static SCB *outputData;
    static SCB *outputSpace;
    static SCB *inputData;
};