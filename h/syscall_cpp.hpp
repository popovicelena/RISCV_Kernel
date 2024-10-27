#ifndef _syscall_cpp_hpp_
#define _syscall_cpp_hpp_
#include "syscall_c.h"
class Thread{
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();
    static void dispatch();
    static void sleep(time_t ){}
protected:
    Thread();
    virtual void run(){};
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;

    static void wrapper(void * p);


};

class Semaphore{

public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();

    int signal();

    int timedwait(time_t t);

    int trywait();

private:
    sem_t myHandle;

};

class Console{

public:
    static void putc(char c);
    static char getc();
};

#endif