#include "../h/syscall_c.h"
#include "../lib/console.h"
//#include "../test/printing.hpp"



void* mem_alloc (size_t size){

    uint64 num = 0x01;
    __asm__ volatile("mv a1, %0"::"r" (size));
    __asm__ volatile("mv a0, %0"::"r" (num));

    __asm__ volatile("ecall");

    uint64 retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));

    return  (void*)retval;


}

int mem_free (void* addr){

    uint64 num = 0x02;
    __asm__ volatile("mv a1, %0"::"r" (addr));
    __asm__ volatile("mv a0, %0"::"r" (num));

    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));

    return  retval;

}

//Niti
int thread_create(thread_t* handle,void(*start_routine)(void*),void* arg){


    uint64 syscall_code = 0x11;
    uint64 arguments[] = {(uint64)handle,(uint64)start_routine,(uint64)arg };
    __asm__ volatile("mv a1, %0"::"r" (arguments));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile ("ecall");

    TCB* retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    if(retval == nullptr) return -1;
    return 0;

}

int thread_exit(){
    int syscall_code = 0x12;
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));
    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return  retval;

}

void thread_dispatch(){
    int syscall_code = 0x13;
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));
    __asm__ volatile("ecall");
}

int thread_start(thread_t handle,void(*start_routine)(void*),void* arg){
    uint64 syscall_code = 0x14;
    uint64 arguments[] = {(uint64)handle,(uint64)start_routine,(uint64) arg };

    __asm__ volatile("mv a1, %0"::"r" (arguments));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile ("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;
}

void thread_destroy(thread_t handle){
    uint64 syscall_code = 0x15;
    uint64 arg = (uint64)handle;
    __asm__ volatile("mv a1, %0"::"r" (arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile ("ecall");


    //__asm__ volatile("mv %0, a0":"=r"(retval));
}

//semafori

int sem_open(sem_t* handle, unsigned init){

    uint64 syscall_code = 0x21;
    uint64 arguments[] ={(uint64)handle,(uint64)init };

    __asm__ volatile("mv a1, %0"::"r" (arguments));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile ("ecall");


    KSemaphore* retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    if(retval == nullptr) return -1;
    return 0;

}

int sem_close(sem_t handle){

    if(handle == nullptr) return -1;
    uint64 syscall_code = 0x22;
    uint64 arg = (uint64) handle;
    __asm__ volatile("mv a1, %0"::"r" (arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;
}

int sem_wait(sem_t id){

    if(id == nullptr ) return -1;
    uint64 syscall_code = 0x23;
    uint64 arg = (uint64) id;
    __asm__ volatile("mv a1, %0"::"r" (arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;

}

int sem_signal(sem_t id){

    if(id == nullptr) return -1;

    uint64 syscall_code = 0x24;
    uint64 arg = (uint64) id;
    __asm__ volatile("mv a1, %0"::"r" (arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;

}

int sem_timedwait(sem_t id, time_t timeout){

    uint64 syscall_code = 0x25;
    uint64 arguments[] = {(uint64)id,(uint64)timeout };
    __asm__ volatile("mv a1, %0"::"r" (arguments));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile ("ecall");

    int retval ;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;

}

int sem_trywait(sem_t id){

    if(id == nullptr)return -1;
    uint64 syscall_code = 0x26;
    uint64 arg = (uint64) id;
    __asm__ volatile("mv a1, %0"::"r" (arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

    int retval;
    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;

}

void sem_destroy(sem_t handle){

    uint64 syscall_code = 0x27;
    uint64 arg = (uint64) handle;

    __asm__ volatile("mv a1,%0"::"r"(arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

}

void putc(char c){

    uint64 syscall_code = 0x41;
    uint64 arg = (uint64)c;
    __asm__ volatile("mv a1,%0"::"r"(arg));
    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

}

char getc(){

    uint64 syscall_code = 0x42;

    __asm__ volatile("mv a0, %0"::"r" (syscall_code));

    __asm__ volatile("ecall");

    char retval ;

    __asm__ volatile("mv %0, a0":"=r"(retval));
    return retval;

}



void time_sleep(time_t ){


}

