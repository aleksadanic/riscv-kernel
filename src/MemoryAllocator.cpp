#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"

void* MemoryAllocator::alloc (size_t blocks) {
    // printString("MemoryAllocator::alloc\n");
    if (blocks == 0) {
        return 0;
    }
    size_t size = blocks * MEM_BLOCK_SIZE;
    AllocatedSegment *curr = 0, *next = firstAllocatedSegment;
    if (size + MEM_BLOCK_SIZE <= (size_t) ((char*) next - (char*) HEAP_START_ADDR)) {
        firstAllocatedSegment = (AllocatedSegment*) HEAP_START_ADDR;
        firstAllocatedSegment->size = size + MEM_BLOCK_SIZE;
        firstAllocatedSegment->next = next;
        return (void*) ((char*) firstAllocatedSegment + MEM_BLOCK_SIZE);
    }
    while (next != (AllocatedSegment*) HEAP_END_ADDR) {
        curr = next;
        next = next->next;
        if (size + MEM_BLOCK_SIZE <= (size_t) ((char*) next - (char*) curr) - curr->size) {
            AllocatedSegment* newSegment = (AllocatedSegment*) ((char*) curr + curr->size);
            curr->next = newSegment;
            newSegment->next = next;
            newSegment->size = size + MEM_BLOCK_SIZE;
            return (void*) ((char*) newSegment + MEM_BLOCK_SIZE);
        }
    }
    return 0;
}

int MemoryAllocator::free (void* address) {
    AllocatedSegment *curr = 0, *next = firstAllocatedSegment;
    while (next != (AllocatedSegment*) HEAP_END_ADDR) {
        if ((char*) next + MEM_BLOCK_SIZE == (char*) address) {
            if (curr) {
                curr->next = next->next;
            } else {
                firstAllocatedSegment = next->next;
            }
            return 0;
        }
        curr = next;
        next = next->next;
    }
    return -1;
}

AllocatedSegment* MemoryAllocator::firstAllocatedSegment = (AllocatedSegment*) HEAP_END_ADDR;