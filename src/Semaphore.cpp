#include "../h/Semaphore.hpp"
#include "../h/tcb.hpp"
#include "../h/scheduler.hpp"


class KSemaphore;

KSemaphore::KSemaphore(KSemaphore **handle, unsigned init){
    *handle = this;
    val = (int)init;
    semclosed = false;
    timeblocked = Timeblocked::getInstance();
}

KSemaphore* KSemaphore::sem_open(KSemaphore **handle, unsigned  init) {

    return new KSemaphore(handle, init);

}

int KSemaphore::sem_close(KSemaphore* handle) {

    handle->setSemclosed(true);
    while (handle->blocked.peekFirst()){

        handle->unblock();
    }

    delete handle;

    return 0;

}

void KSemaphore::block() {
    TCB* oldRunning = TCB::running;
    oldRunning->setBlocked(true);
    blocked.addLast(oldRunning);
    TCB::yield();

}

void KSemaphore::unblock() {

    if(blocked.peekFirst()){
        TCB* t = blocked.removeFirst();
        t->setBlocked(false);
        Scheduler::put(t);
    }

}



int KSemaphore::wait() {

    if(semclosed){return -1;}

    if(--val < 0){
       block();
    }

    if(semclosed){ return -1; }
    return 0;

}

int KSemaphore::signal(){

    if(semclosed) return -1;

    if(++val<=0){
        unblock();
    }

    return 0;

}

int KSemaphore::sem_wait_wrapper(KSemaphore *id) {

    int retval = id->wait();
    return retval;
}

int KSemaphore::sem_signal_wrapper(KSemaphore *id) {

    int retval = id->signal();
    return retval;
}

int KSemaphore::sem_timedwait_wrapper(KSemaphore* id, time_t timeout){

    int retval = id->timewait(timeout);
    return retval;
}


int KSemaphore::timewait(time_t timeout) {

    if(--val<0){
        TCB::running->setTimewait(timeout);
        TCB::running->setBlockedon(this);

        List<TCB>& b = timeblocked->blocked;
        b.addLast(TCB::running);//svi blokirani na odredjeno vreme

        block();
    }







    if(semclosed) {

        TCB::running->setTimewait(-1);
        TCB::running->setBlockedon(nullptr);
        return -1;
    }
    if(TCB::running->getTimeWait() == 0){

        TCB::running->setTimewait(-1);
        TCB::running->setBlockedon(nullptr);
        return -2;
    }
    TCB::running->setTimewait(-1);
    TCB::running->setBlockedon(nullptr);
    return 0;

}

int KSemaphore::trywait() {

    if(semclosed) return -1;
    if(--val<0){
        ++val;
        return 0;
    }else{
        return 1;
    }

}

int KSemaphore::sem_trywait_wrapper(KSemaphore *id) {

    int retval = id->trywait();
    return retval;

}

void KSemaphore::sem_destroy(KSemaphore* handle){
    delete handle;
}



