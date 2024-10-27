#include "../h/memoryallocator.hpp"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/Semaphore.hpp"
#include "../test/printing.hpp"
//bool finisheda = false;

void userMain();

void userMainWrapper(void *){
    userMain();
    //finisheda = true;
}
int main(){

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    MemoryAllocator::initializeHead();
   TCB* handlemain;
    TCB::createThread(&handlemain,nullptr, nullptr, nullptr);
    TCB::running=handlemain;
    TCB* usermainhandle;
    thread_create(&usermainhandle,&userMainWrapper, nullptr);
    while(!usermainhandle->isFinished()){
        thread_dispatch();
    }

    return 0;


}