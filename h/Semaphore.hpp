#ifndef _semaphore_hpp_
#define _semaphore_hpp_
#include "../lib/hw.h"
#include "memoryallocator.hpp"
#include "list.hpp"
#include "tcb.hpp"
#include "timeblocked.hpp"

class KSemaphore{

private:
    int val;
    List<TCB>blocked;
    bool semclosed;
    Timeblocked* timeblocked;


    KSemaphore(KSemaphore **handle,unsigned init);

    void block();
    void unblock();




public:

    static KSemaphore* sem_open(KSemaphore** handle, unsigned init);
    static int sem_close(KSemaphore* handle);
    static int sem_wait_wrapper(KSemaphore* id);
    static int sem_signal_wrapper(KSemaphore* id);
    static int sem_timedwait_wrapper(KSemaphore* id,time_t timeout);
    static int sem_trywait_wrapper(KSemaphore* id);
    static void sem_destroy(KSemaphore* handle);


     int wait();
     int signal();
     int timewait(time_t timeout);
     int trywait();

     List<TCB>& getBlocked(){ return blocked; }

    void setSemclosed(bool value) {semclosed = value; }

    bool getSemclosed() const { return semclosed;}

    void* operator new (size_t n){
        return MemoryAllocator::kmem_alloc(n);
    }
    void* operator new[] (size_t n){
        return MemoryAllocator::kmem_alloc(n);
    }
    void operator delete (void* p){
        MemoryAllocator::kmem_free(p);
    }
    void operator delete[] (void* p){
        MemoryAllocator::kmem_free(p);
    }




};

#endif //_semaphore_hpp_