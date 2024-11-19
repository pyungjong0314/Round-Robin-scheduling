#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "list.h"
#include "run.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 스레드 생성 함수
int thread_create(thread_t *thread, thread_attr_t *attr,
                  void *(*start_routine)(void *), void *arg) {

    // TCB 생성
    Thread *TCB = make_TCB(thread);

    // WrapperArg 설정
    WrapperArg *wrapperArg = (WrapperArg *)malloc(sizeof(WrapperArg));
    wrapperArg->funcPtr = start_routine;
    wrapperArg->funcArg = arg;
    wrapperArg->pThread = TCB;

    // Wrapper 함수를 실행하며 스레드 생성
    int result = pthread_create(thread, attr, wrapperFunc, wrapperArg);
    TCB->tid = *thread;

    // result가 0이 아니면 에러
    return result;
}

int thread_join(thread_t thread, void **retval) {

    // join하는 스레드를 Ready Queue에서 검색
    Thread *child_thread = find_ready(Ready_Queue, thread);
    // 스레드가 없으면 Zombie Queue 검사
    if ((child_thread == NULL)) {
        // Ready Queue에는 없지만 zombie 상태인 스레드를 제거
        child_thread = find_zombie(Zombie_Queue, thread);
        // Join 실패
        if (child_thread == NULL) {
            return -1;
        }
    }

    else {
        // 스레드 wait 상태
        current_thread->status = 2;
        current_thread->bRunnable = 0;

        // Wait Queue 삽입
        push_wait(Wait_Queue, current_thread);

        current_thread = NULL;

        // 스레드 join 상태
        thread_to_join(child_thread);

        // join에서 깨어나면 ready queue 맨 뒤에 삽입
        Thread *parent_thread = find_wait(Wait_Queue, thread_self());

        // 깨어난 스레드 ready 상태
        parent_thread->status = 1;
        thread_resume(parent_thread->tid);

        thread_to_ready2(parent_thread);
    }

    // retval 종료 값 저장
    *retval = child_thread->pExitCode;

    // 자식 프로세스 삭제
    remove_thread_zombie(Zombie_Queue, child_thread);
    free(child_thread);
    return 0;
}

// 스레드 wait 상태
int thread_suspend(thread_t tid) {
    // Ready queue에서 wait할 스레드 검색
    Thread *suspend_thread = find_ready(Ready_Queue, tid);

    // 현재 동작하는 스레드
    if (suspend_thread == NULL) {
        puts("fail suspend");
    } else if (suspend_thread->tid != tid) {
        puts("Not match");
    }

    // Ready Queue 1개
    else if (Ready_Queue->length == 1) {
        Ready_Queue->front_ready_queue = current_thread;
        current_thread = NULL;

        // Wait Queue 삽입
        push_wait(Wait_Queue, suspend_thread);
        //  wait thread 상태 변경
        suspend_thread->status = 2;
    }
    // Ready Queue에 존재하는 스레드
    else {
        // Ready Queue에서 Wait할 스레드 삭제
        remove_thread_ready(Ready_Queue, suspend_thread);
        // Wait Queue 삽입
        push_wait(Wait_Queue, suspend_thread);
        //  wait thread 상태 변경
        suspend_thread->status = 2;
    }

    return 0;
}

// tid가 없으면 -1 return, 정상 종료 0
int thread_resume(thread_t tid) {
    // Wait Queue에서 tid 검색
    Thread *resume_thread = find_wait(Wait_Queue, tid);

    // 에러 발생
    if (resume_thread == NULL) {
        puts("thread can't resume");
        return -1;
    }

    // Wait Queue 삭제
    remove_thread_wait(Wait_Queue, resume_thread);

    // Ready Queue 삽입
    push_ready(Ready_Queue, resume_thread);

    // 상태 변경
    resume_thread->status = 1;

    return 0;
}

thread_t thread_self() {
    thread_t tid = pthread_self();
    return tid;
}

// 스레드 종료
int thread_exit(void *retval) {
    int *exit_value = (int *)retval;

    // 종료할 스레드 상태 변경
    Thread *exit_thread = current_thread;
    exit_thread->status = 3;

    // 스레드 좀비 상태
    thread_to_zombie(exit_thread);

    current_thread = NULL;

    // 종료 값 저장
    exit_thread->pExitCode = retval;

    push_zombie(Zombie_Queue, exit_thread);

    return *exit_value;
}
