//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"


TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB* TCB::createThread(TCB** handle,Body body,void* arg, void* stack_space)
{
    return new TCB(handle,body,arg,stack_space);

}

int TCB::startThread(TCB* handle, TCB::Body body, void *arg) {

    if(handle == nullptr) return -1;
    if(handle->getStarted()) return -2;
    handle->setBody(body);
    handle->setArg(arg);
    handle->setStarted(true);
    Scheduler::put(handle);

    return 0;
}



void TCB::yield()
{
    Riscv::pushRegisters();
    TCB::dispatch();
    Riscv::popRegisters();
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

TCB:: TCB(TCB** handle,Body body,void* arg, void* stack_space) :
        body(body),
        arg(arg),
        stack((uint64*)stack_space),
        context({(uint64) &threadWrapper,
                 stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE]: 0
                }),
        timeSlice(DEFAULT_TIME_SLICE),
        finished(false),
        blocked(false),
        timewait(-1),
        blockedOn(nullptr),
        started(false)
{
    *handle = this;
    if (body != nullptr) { Scheduler::put(this); }
}

void TCB::destroyThread(TCB* handle){
    delete handle;
}
