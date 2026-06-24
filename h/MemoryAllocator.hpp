#include "../lib/hw.h"

struct AllocatedSegment {
    size_t size;
    AllocatedSegment* next;
};

class MemoryAllocator {
public:
    static void* alloc (size_t blocks);
    static int free (void* address);
    static void init ();

private:
    static AllocatedSegment* firstAllocatedSegment;
};