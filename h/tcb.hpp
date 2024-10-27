//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "memoryallocator.hpp"
class KSemaphore;

// Thread Control Block
class TCB
{
public:

    ~TCB() {delete[] stack;}

    bool isFinished() const { return finished; }

    bool isBlocked() const {return blocked; }

    void setFinished(bool value) { finished = value; }

    void setBlocked(bool value) { blocked = value; }

    int getTimeWait(){return timewait; }

    void setTimewait(int timeout){timewait = timeout; }

    void setBlockedon(KSemaphore* id){ blockedOn = id;}


    KSemaphore* getBlockedon() const{return blockedOn; }


    void setArg(void* arg){ this->arg = arg;}
    void setStarted(bool b){started = b;}
    bool getStarted() const {return started;}

   // bool isSemclosed() const {return semclosed; }

    uint64 getTimeSlice() const { return timeSlice; }

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

    static void dispatch();

    using Body = void (*)(void*);

    void setBody(Body body){ this->body = body;}

    static TCB* createThread(TCB** handle,Body body,void* arg,void* stack_space);

    static int startThread(TCB* handle,Body body,void* arg);

    static void destroyThread(TCB* handle);

    static void yield();//ne treba vrv

    static TCB *running;

private:
    TCB(TCB** handle,Body body,void* arg, void* stack_space) ;


    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;//koliko bi perioda tajmera nit trebala da se izvrsava
    bool finished;
    bool blocked;
    int timewait;
    KSemaphore* blockedOn;
    bool started;
    //bool semclosed;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    //static void dispatch();

    static uint64 timeSliceCounter;

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
