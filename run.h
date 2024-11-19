#ifndef _RUN_H_
#define _RUN_H_

#include "Thread.h"

// 현재 동작 중인 스레드
extern Thread *current_thread;

// 멈출 스레드
extern Thread *stop_thread;

// Ready, Run 상태 mutex
extern pthread_mutex_t mutex;

// join, zombie 상태 mutex
extern pthread_mutex_t zombie_mutex;

void *wrapperFunc(void *arg);

void thread_to_ready(int signum);

void thread_to_ready2(Thread *ready_thread);

void thread_to_run(Thread *run_thread);

void thread_to_join(Thread *child_thread);

void thread_to_zombie(Thread *zombie_thread);

Thread *make_TCB(pthread_t *entry_tid);

#endif
