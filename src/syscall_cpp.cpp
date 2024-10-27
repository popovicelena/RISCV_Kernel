#include "../h/syscall_c.h"
#include "../h/tcb.hpp"
#include "../h/Semaphore.hpp"
#include "../h/syscall_cpp.hpp"


Thread::Thread(void (*body)(void*), void* arg){

        this->body = body;
        this->arg = arg;
        thread_create(&myHandle, nullptr, nullptr);

    }

    Thread::~Thread(){ thread_destroy(myHandle);}

    int Thread::start(){

        int retval = thread_start(myHandle,body,arg);
        return retval;
    }

   void Thread::dispatch(){
        thread_dispatch();
    }

    Thread::Thread(): body(wrapper),arg(this){
        thread_create(&myHandle, nullptr, nullptr);
    }


     void Thread::wrapper(void * p){
        Thread* thr = (Thread*)p;
        if(thr) thr->run();
    }






    Semaphore::Semaphore(unsigned init){
        sem_open(&myHandle,init);
    }
    Semaphore::~Semaphore(){
        sem_close(myHandle);
    }

    int Semaphore::wait(){
        int retval = sem_wait(myHandle);
        return retval;
    }

    int Semaphore::signal(){
        int retval = sem_signal(myHandle);
        return retval;
    }

    int Semaphore::timedwait(time_t t){
        int retval = sem_timedwait(myHandle,t);
        return retval;
    }

    int Semaphore::trywait(){
        int retval = sem_trywait(myHandle);
        return retval;
    }

void Console::putc(char c){
    ::putc(c);
}
char Console::getc() {
    char a = ::getc();
    return a;
}



