#ifndef _syscall_c_hpp_
#define _syscall_c_hpp_
#include "../lib/hw.h"

class TCB;
typedef TCB* thread_t;
int thread_create(thread_t* handle,void(*start_routine)(void*),void* arg);
int thread_exit();
void thread_dispatch();
int thread_start(thread_t handle,void(*start_routine)(void*),void* arg);
void thread_destroy(thread_t handle);

class KSemaphore;
typedef KSemaphore* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);
void sem_destroy(sem_t handle);

void* mem_alloc(size_t size);
int mem_free(void*);

void putc(char c);
char getc();




void time_sleep(time_t );

#endif