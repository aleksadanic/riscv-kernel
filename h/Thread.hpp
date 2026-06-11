#include "../lib/hw.h"

struct Context {
    uint64 sp;
    uint64 ra;
    uint64 s[12];
    uint64 a[8];
    uint64 t[7];
    uint64 gp;
    uint64 tp;

    uint64 kernel_sp;
};

class Thread {
public:
    Thread* next;
    Context* context;
};