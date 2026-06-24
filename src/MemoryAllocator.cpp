#include "../h/MemoryAllocator.hpp"

void* MemoryAllocator::alloc (size_t blocks) {
    if (blocks == 0) {
        return 0;
    }
    void* heapStartAddr = (void*) ((((uint64) HEAP_START_ADDR) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE);
    size_t size = blocks * MEM_BLOCK_SIZE;
    AllocatedSegment *curr = 0, *next = firstAllocatedSegment;
    if (size + MEM_BLOCK_SIZE <= (size_t) ((char*) next - (char*) heapStartAddr)) {
        firstAllocatedSegment = (AllocatedSegment*) heapStartAddr;
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

void MemoryAllocator::init () {
    firstAllocatedSegment = (AllocatedSegment*) HEAP_END_ADDR;
}

AllocatedSegment* MemoryAllocator::firstAllocatedSegment = 0;