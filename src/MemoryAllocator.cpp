#include "../h/MemoryAllocator.hpp"

void* MemoryAllocator::alloc (size_t blocks) {
    size_t size = blocks * MEM_BLOCK_SIZE;
    FreeSegment *curr = 0, *next = firstFreeSegment;
    while (next != 0) {
        if (next->size >= size) {
            if (next->size - size >= sizeof (FreeSegment)) {
                FreeSegment* newNext = (FreeSegment*) ((size_t) next + size);
                newNext->size = next->size - size;
                newNext->next = next->next;
                if (curr) {
                    curr->next = newNext;
                } else {
                    firstFreeSegment = newNext;
                }
            } else {
                if (curr) {
                    curr->next = next->next;
                } else {
                    firstFreeSegment = next->next;
                }
            }
            return (void*) next;
        }
        curr = next;
        next = next->next;
    }
    return 0;
}

MemoryAllocator& MemoryAllocator::getInstance () {
    static MemoryAllocator instance;
    return instance;
}

MemoryAllocator::MemoryAllocator () {
    firstFreeSegment = (FreeSegment*) HEAP_START_ADDR;
    firstFreeSegment->size = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR;
    firstFreeSegment->next = 0;
}