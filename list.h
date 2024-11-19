#ifndef __LIST_H__
#define __LIST_H__

#include "Thread.h"

// Ready Queue 구조체
typedef struct ready_queue {
    int length;
    Thread *front_ready_queue;
    Thread *rear_ready_queue;
} ready_queue;

// Wait Queue 구조체
typedef struct waiting_queue {
    int length;
    Thread *front_wait_queue;
    Thread *rear_wait_queue;
} wait_queue;

// Zombie Queue 구조체
typedef struct zombie_queue {
    int length;
    Thread *front_zombie_queue;
    Thread *rear_zombie_queue;
} zombie_queue;

// Ready Queue 초기화
ready_queue *init_ready_queue();
// Ready Queue 삽입
void push_ready(ready_queue *RQ, Thread *thread_entry);
// Ready Queue 삭제
void pop_ready(ready_queue *RQ);
// Ready Queue 출력
void show_ready(ready_queue *RQ);
// 원하는 tid Ready Queue에서 검색
Thread *find_ready(ready_queue *RQ, thread_t target_tid);
// 원하는 TCB Ready Queue에서 삭제
void remove_thread_ready(ready_queue *RQ, Thread *remove_thread);

// Wait Queue 초기화
wait_queue *init_wait_queue();
// Wait Queue 삽입
void push_wait(wait_queue *WQ, Thread *thread_entry);
// Wait Queue 삭제
void pop_wait(wait_queue *WQ);
// Wait Queue 출력
void show_wait(wait_queue *WQ);
// 원하는 tid Wait Queue에서 검색
Thread *find_wait(wait_queue *WQ, thread_t target_tid);
// 원하는 TCB Wait Queue에서 삭제
void remove_thread_wait(wait_queue *WQ, Thread *remove_thread);

// Zombie Queue 초기화
zombie_queue *init_zombie_queue();
// Zombie Queue 삽입
void push_zombie(zombie_queue *ZQ, Thread *thread_entry);
// Zombie Queue 삭제
void pop_zombie(zombie_queue *ZQ);
// Zombie Queue 출력
void show_zombie(zombie_queue *ZQ);
// 원하는 tid Zombie Queue에서 검색
Thread *find_zombie(zombie_queue *ZQ, thread_t target_tid);
// 원하는 TCB Zombie Queue에서 삭제
void remove_thread_zombie(zombie_queue *ZQ, Thread *remove_thread);

// 전역 변수
extern ready_queue *Ready_Queue;

extern wait_queue *Wait_Queue;

extern zombie_queue *Zombie_Queue;

#endif
