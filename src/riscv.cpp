#include "../h/riscv.hpp"
#include "../h/memoryallocator.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/Semaphore.hpp"
#include "../h/timeblocked.hpp"
#include "../test/printing.hpp"


void Riscv::popSppSpie()
{
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        //read value from a0
        uint8 a0_val;
        __asm__ volatile("mv %0, a0":"=r" (a0_val));
        if(a0_val == 0x01){
            size_t sz;
            __asm__ volatile("mv %0, a1":"=r" (sz));
           MemoryAllocator::kmem_alloc(sz);
            __asm__ volatile("sd a0, 80(s0)");
        }else if(a0_val == 0x02){
            void* adr;
            __asm__ volatile("mv %0, a1":"=r" (adr));
            MemoryAllocator::kmem_free(adr);
            __asm__ volatile("sd a0, 80(s0)");
        }else if(a0_val == 0x11){
            uint64* arguments;
            __asm__ volatile("mv %0, a1":"=r" (arguments));
            void* stack_space = MemoryAllocator::kmem_alloc(DEFAULT_STACK_SIZE*sizeof (uint64));
            TCB::createThread((TCB**)arguments[0],(TCB::Body)arguments[1],(void*)arguments[2],(void*)stack_space);

            __asm__ volatile("sd a0, 80(s0)");


        }else if(a0_val == 0x13) {

            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        }else if(a0_val == 0x12){

            TCB::running->setFinished(true);
            TCB::dispatch();

            __asm__ volatile("li a0, 0");
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x21){

            uint64* arguments;
            __asm__ volatile("mv %0, a1":"=r" (arguments));
            KSemaphore::sem_open((KSemaphore**)arguments[0], (unsigned)arguments[1]);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x22){

            uint64 handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            KSemaphore::sem_close((KSemaphore*) handle);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x23){

            uint64 id;
            __asm__ volatile("mv %0, a1":"=r"(id));
            KSemaphore::sem_wait_wrapper((KSemaphore*) id);
            __asm__ volatile("sd a0, 80(s0)");


        }else if(a0_val == 0x24){

            uint64 id;
            __asm__ volatile("mv %0, a1":"=r"(id));
//            Semaphore* handle = (Semaphore*)id;
//            handle->wait()
            KSemaphore::sem_signal_wrapper((KSemaphore*) id);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x25){

            uint64* arguments;
            __asm__ volatile("mv %0, a1":"=r" (arguments));
//            Semaphore* handle = (Semaphore*)arguments[0];
//            handle->timewait((time_t)arguments[1]);

            KSemaphore::sem_timedwait_wrapper((KSemaphore*)arguments[0], (time_t)arguments[1]);

            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x26){

            uint64 handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            KSemaphore::sem_trywait_wrapper((KSemaphore*) handle);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x14){

            uint64* arguments;
            __asm__ volatile("mv %0, a1":"=r" (arguments));
            TCB::startThread((TCB*)arguments[0],(TCB::Body)arguments[1],(void*)arguments[2]);
            __asm__ volatile("sd a0, 80(s0)");


        }else if(a0_val == 0x15){

            uint64 arg;
            __asm__ volatile("mv %0, a1":"=r" (arg));
           TCB::destroyThread((TCB*)arg);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x27){

            uint64 arg;
            __asm__ volatile("mv %0, a1":"=r" (arg));
            KSemaphore::sem_destroy((KSemaphore*)arg);
            __asm__ volatile("sd a0, 80(s0)");

        }else if(a0_val == 0x41){

            uint64 arg;
            __asm__ volatile("mv %0, a1":"=r" (arg));
            __putc((char)arg);
            __asm__ volatile("sd a0, 80(s0)");


        }else if(a0_val == 0x42){

            char c = __getc();
            __asm__ volatile("mv a0, %0"::"r" (c));
            __asm__ volatile("sd a0, 80(s0)");

        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
       // uint64 volatile sepc = r_sepc();//sacuva sepc na steku
       // uint64 volatile sstatus = r_sstatus();//sacuva sstatus na steku
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);

        //timedwait
    /*
        TCB::timeSliceCounter++;


        Timeblocked* queue = Timeblocked::getInstance();
        TCB* first = queue->blocked.removeFirst();

        if(first!= nullptr){
            first->setTimewait(first->getTimeWait() - 1);
            if(first->getTimeWait() == 0){
                first->getBlockedon()->getBlocked().removeElem(first);
                first->setBlocked(false);
                Scheduler::put(first);
                //__putc('l');
            }else{
                queue->blocked.addLast(first);
            }

        }

        if(queue->blocked.peekFirst()!= nullptr)//__putc('x');
        while(queue->blocked.peekFirst()){
           // __putc('w');
            if(queue->blocked.peekFirst() == first) break;
            TCB* t = queue->blocked.removeFirst();
            t->setTimewait(t->getTimeWait() - 1);
            if(t->getTimeWait() == 0){

                queue->blocked.removeElem(t);
                t->getBlockedon()->getBlocked().removeElem(t);
                t->setBlocked(false);
                Scheduler::put(t);

            }
            queue->blocked.addLast(t);


        }

        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {

            TCB::timeSliceCounter = 0;
            TCB::dispatch();


        }
        w_sstatus(sstatus);
        w_sepc(sepc);
*/

    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();


    }
    else
    {
        printInt(r_sepc(), 16);
        printString("scause: ");
        printInt(scause);
        printString(" stval: ");
        printInt(r_stval());
        printString("\n");

    }
}