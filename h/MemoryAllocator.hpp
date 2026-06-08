#include "../lib/hw.h"

struct AllocatedSegment {
    size_t size;
    AllocatedSegment* next;
};

class MemoryAllocator {
public:
    void* alloc (size_t blocks);
    int free (void* address);

    static MemoryAllocator& getInstance ();

private:
    AllocatedSegment* firstAllocatedSegment;
    MemoryAllocator ();
};