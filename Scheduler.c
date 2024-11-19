#include "Scheduler.h"
#include "list.h"
#include "run.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

Thread *current_thread = NULL;
Thread *stop_thread = NULL;

int RunScheduler(void) {
    while (1) {
        sleep(TIMESLICE);
        // ready queue에 아무것도 없는 경우
        if (Ready_Queue->length == 0) {
        }
        // 돌아가는 thread가 없는 경우
        else if (current_thread == NULL) {
            // Ready queue 제일 앞 스레드
            Thread *next_run_thread = Ready_Queue->front_ready_queue;
            // 동작할 스레드 상태 변경
            next_run_thread->bRunnable = 1;
            next_run_thread->status = 0;
            // Ready queue에서 제거
            pop_ready(Ready_Queue);

            // 쓰레드 동작
            current_thread = next_run_thread;
            thread_to_run(next_run_thread);

        }
        // 정상
        else {
            // 동작할 쓰레드 상태 변경
            Thread *next_run_thread = Ready_Queue->front_ready_queue;
            next_run_thread->bRunnable = 1;
            next_run_thread->status = 0;
            pop_ready(Ready_Queue);

            // 멈출 쓰레드 상태 변경
            stop_thread = current_thread;
            stop_thread->bRunnable = 0;
            // 멈출 스레드 Ready Queue 제일 뒤에 삽입
            pthread_t stop_tid = stop_thread->tid;
            push_ready(Ready_Queue, stop_thread);

            // 다음 쓰레드 동작
            current_thread = next_run_thread;
            thread_to_run(next_run_thread);

            // 멈출 쓰레드에게 시그널 전송
            pthread_kill(stop_tid, SIGUSR1);
        }
        // show_ready(Ready_Queue);
        // show_wait(Wait_Queue);
    }
}

// 함수 사용 X
void __ContextSwitch(Thread *pCurThread, Thread *pNewThread) {}
