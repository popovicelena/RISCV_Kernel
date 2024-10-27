
#ifndef _timeblocked_hpp_
#define _timeblocked_hpp_
#include "list.hpp"
#include "tcb.hpp"
#include "memoryallocator.hpp"

class Timeblocked {

private:

    static Timeblocked *instancePtr;

    Timeblocked() {

    }

    void *operator new(size_t n) {
        return MemoryAllocator::kmem_alloc(n);
    }

    void *operator new[](size_t n) {
        return MemoryAllocator::kmem_alloc(n);
    }

    void operator delete(void *p) {
        MemoryAllocator::kmem_free(p);
    }

    void operator delete[](void *p) {
        MemoryAllocator::kmem_free(p);
    }

public:
    List<TCB> blocked;

    Timeblocked(const Timeblocked &obj) = delete;

    static Timeblocked *getInstance();

};

#endif