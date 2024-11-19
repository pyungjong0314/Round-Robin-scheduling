#include "run.h"
#include "list.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// TCB를 만드는 함수, TCB 반환
Thread *make_TCB(pthread_t *entry_tid) {
    // Thread control block 생성
    Thread *new_thread = (Thread *)malloc(sizeof(Thread));

    // mutex와 pthread_cond 설정 설정
    new_thread->readyMutex = mutex;
    pthread_cond_t new_thread_cond = PTHREAD_COND_INITIALIZER;
    new_thread->readyCond = new_thread_cond;
    new_thread->zombieMutex = zombie_mutex;
    pthread_cond_init(&(new_thread->zombieCond), NULL);

    // TCB READY 상태 설정
    new_thread->status = THREAD_STATUS_READY;
    new_thread->tid = *entry_tid;

    // exitcode
    new_thread->bRunnable = 0; // false
    new_thread->parentTid = pthread_self();

    // Zombie
    new_thread->bZombie = 0;

    // ready queue에 TCB 저장
    push_ready(Ready_Queue, new_thread);

    return new_thread;
}

void *wrapperFunc(void *arg) {
    // signal 설정
    signal(SIGUSR1, thread_to_ready);

    WrapperArg *pArg = (WrapperArg *)arg;

    // TCB tid 초기화
    Thread *pTh = pArg->pThread;
    pTh->tid = thread_self();

    // 함수 초기화
    void *(*Func)(void *) = pArg->funcPtr;
    void *Arg = pArg->funcArg;

    void *ret;

    // Thread ready 상태
    thread_to_ready2(pTh);

    // Thread 함수 실행
    ret = Func(Arg);
    return ret;
}

// signal을 받으면 스레드를 ready 상태로 변경하는 함수
void thread_to_ready(int signum) {
    if (signum == SIGUSR1) {
        // Ready Queue 마지막에 삽입 되어있는 스레드를 정지
        Thread *pTh = stop_thread;
        pTh->status = 1;
        pthread_mutex_lock(&(pTh->readyMutex));
        while (pTh->bRunnable == 0) {
            pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
        }
        pthread_mutex_unlock(&pTh->readyMutex);
    }
}

// 스레드를 생성하면서 ready 상태로 변경
void thread_to_ready2(Thread *ready_thread) {
    pthread_mutex_lock(&ready_thread->readyMutex);
    while (ready_thread->bRunnable == 0) {
        pthread_cond_wait(&ready_thread->readyCond, &ready_thread->readyMutex);
    }
    pthread_mutex_unlock(&ready_thread->readyMutex);
}

// 스레드를 실행
void thread_to_run(Thread *run_thread) {
    pthread_mutex_lock(&(run_thread->readyMutex));
    run_thread->bRunnable = 1;
    pthread_cond_signal(&(run_thread->readyCond));

    pthread_mutex_unlock(&(run_thread->readyMutex));
}

// 스레드 좀비 상태
void thread_to_zombie(Thread *zombie_thread) {
    pthread_mutex_lock(&(zombie_thread->zombieMutex));
    // 스레드 상태 변경
    zombie_thread->bZombie = 1;
    // join하는 함수에게 cond_signal
    pthread_cond_signal(&(zombie_thread->zombieCond));
    pthread_mutex_unlock(&(zombie_thread->zombieMutex));
}

// 스레드를 join 상태로 변결
void thread_to_join(Thread *child_thread) {
    pthread_mutex_lock(&(child_thread->zombieMutex));
    while (child_thread->bZombie == 0) {
        pthread_cond_wait(&child_thread->zombieCond,
                          &child_thread->zombieMutex);
    }

    pthread_mutex_unlock(&(child_thread->zombieMutex));
}
