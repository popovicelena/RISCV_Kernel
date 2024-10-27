#ifndef _memoryallocator_h_
#define _memoryallocator_h_
#include "../lib/hw.h"

class MemoryAllocator{
private:
    struct BlockHeader{
        BlockHeader* next;
        BlockHeader* prev;
        size_t size;
    };
    static BlockHeader* fmem_head;
    static BlockHeader* amem_head;


public:


    static void initializeHead();
    static void* kmem_alloc(size_t size);
    static int kmem_free(void*);


};
#endif