#include "../lib/hw.h"

struct FreeSegment {
    size_t size;
    FreeSegment* next;
};

class MemoryAllocator {
public:
    void* alloc (size_t blocks);
    int mem_free (void* address);

    static MemoryAllocator& getInstance ();

private:
    FreeSegment* firstFreeSegment;
    MemoryAllocator ();
};