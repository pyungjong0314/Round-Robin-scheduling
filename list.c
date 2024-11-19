#include "list.h"
#include <stdio.h>
#include <stdlib.h>

Thread *ReadyQHead;
Thread *ReadyQTail;

Thread *WaitQHead;
Thread *WaitQTail;

ready_queue *init_ready_queue() {
    // ready queue 생성
    ready_queue *RQ = (ready_queue *)malloc(sizeof(ready_queue));
    if (RQ == NULL) { // 메모리 할당 실패
        return NULL;
    }

    RQ->length = 0;
    RQ->front_ready_queue = NULL;
    RQ->rear_ready_queue = NULL;

    // ReadyQ 설정
    ReadyQHead = RQ->front_ready_queue;
    ReadyQTail = RQ->rear_ready_queue;

    return RQ;
}

void push_ready(ready_queue *RQ, Thread *thread_entry) {
    // 첫번째 노드
    if (RQ->front_ready_queue == NULL) {
        // Queue의 처음과 마지막 설정
        RQ->front_ready_queue = thread_entry;
        RQ->rear_ready_queue = thread_entry;
        // Thread의 이전과 이후 Thread 설정
        thread_entry->pPrev = NULL;
        thread_entry->pNext = NULL;
        // Queue 길이
        RQ->length = 1;
    } else {
        // 마지막 Thread의 이후 노드 설정
        RQ->rear_ready_queue->pNext = thread_entry;
        // 삽입하는 Thread의 이전 노드 설정
        thread_entry->pPrev = RQ->rear_ready_queue;
        thread_entry->pNext = NULL;
        // Queue의 마지막에 Thread 삽입
        RQ->rear_ready_queue = thread_entry;
        RQ->length++;
    }

    // ReadyQ 설정
    ReadyQHead = RQ->front_ready_queue;
    ReadyQTail = RQ->rear_ready_queue;
}

void pop_ready(ready_queue *RQ) {
    if (RQ->front_ready_queue == NULL) { // error
        exit(-1);
    } else if (RQ->length == 1) {
        // Ready Queue TCB 1개
        Thread *pop_thread = RQ->front_ready_queue;
        // 삭제한 Thread 설정 변경
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;

        // Ready queue 변경
        RQ->front_ready_queue = NULL;
        RQ->length = 0;
    } else {
        // 첫 번째 Thread 삭제, Queue 헤드 업데이트
        Thread *pop_thread = RQ->front_ready_queue;
        RQ->front_ready_queue = pop_thread->pNext;
        RQ->front_ready_queue->pPrev = NULL;
        // 삭제한 Thread 설정 변경
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;
        // Queue 길이 설정
        RQ->length--;
    }

    // ReadyQ 설정
    ReadyQHead = RQ->front_ready_queue;
    ReadyQTail = RQ->rear_ready_queue;
}

// Queue에 존재하는 모든 Thread의 tid 출력
void show_ready(ready_queue *RQ) {
    Thread *cursor;
    // Queue가 비었다면
    if (RQ->front_ready_queue == NULL) {
        printf("READY QUEUE EMPTY\n");
        return;
    }
    printf("READY Thread ID:");
    // 스레도 tid 출력
    for (cursor = RQ->front_ready_queue; cursor != NULL;
         cursor = cursor->pNext) {
        printf("%ld ", cursor->tid);
    }
    printf("\n");
}

// target tid가 있는 Thread 포인터를 반환 (NULL 실패)
Thread *find_ready(ready_queue *RQ, thread_t target_tid) {
    Thread *cursor = RQ->front_ready_queue;
    // Queue 처음부터 tid 검색
    while ((cursor != NULL) && (cursor->tid != target_tid)) {
        cursor = cursor->pNext;
    }

    if (cursor == NULL) {
        puts("tid no exist");
        return NULL;
    }

    return cursor;
}

void remove_thread_ready(ready_queue *RQ, Thread *remove_thread) {
    // 큐에 한개 존재하는 경우
    if (RQ->length == 1) {
        RQ->front_ready_queue = NULL;
        RQ->rear_ready_queue = NULL;
    }
    // 큐 처음에 존재
    else if (remove_thread->pPrev == NULL) {
        remove_thread->pNext->pPrev = NULL;
        RQ->front_ready_queue = remove_thread->pNext;
    }
    // 큐 마지막에 존재
    else if (remove_thread->pNext == NULL) {
        remove_thread->pPrev->pNext = NULL;
        RQ->rear_ready_queue = remove_thread->pPrev;
    }
    // 큐 중간에 위치하는 경우
    else {
        remove_thread->pPrev->pNext = remove_thread->pNext;
        remove_thread->pNext->pPrev = remove_thread->pPrev;
    }
    remove_thread->pPrev = NULL;
    remove_thread->pNext = NULL;
    RQ->length--;

    // ReadyQ 설정
    ReadyQHead = RQ->front_ready_queue;
    ReadyQTail = RQ->rear_ready_queue;
}

wait_queue *init_wait_queue() {
    // waiting queue 생성
    wait_queue *WQ = (wait_queue *)malloc(sizeof(wait_queue));
    if (WQ == NULL) { // 메모리 할당 실패
        return NULL;
    }

    WQ->length = 0;
    WQ->front_wait_queue = NULL;
    WQ->rear_wait_queue = NULL;

    // WaitQ 설정
    WaitQHead = WQ->front_wait_queue;
    WaitQTail = WQ->rear_wait_queue;

    return WQ;
}

void push_wait(wait_queue *WQ, Thread *thread_entry) {
    // 첫번째 Thread
    if (WQ->front_wait_queue == NULL) {
        // Queue의 처음과 마지막 Thread 설정
        WQ->front_wait_queue = thread_entry;
        WQ->rear_wait_queue = thread_entry;
        // 삽입한 Thread 이전과 이후 노드 설정
        thread_entry->pPrev = NULL;
        thread_entry->pNext = NULL;
        // Queue 길이
        WQ->length = 1;
    } else {
        // 마지막 Thread의 이후 Thread 설정
        WQ->rear_wait_queue->pNext = thread_entry;
        // 삽입하는 Thread의 이전 Thread 설정
        thread_entry->pPrev = WQ->rear_wait_queue;
        thread_entry->pNext = NULL;
        // Queue의 마지막에 Thread 삽입
        WQ->rear_wait_queue = thread_entry;
        WQ->length++;
    }

    // WaitQ 설정
    WaitQHead = WQ->front_wait_queue;
    WaitQTail = WQ->rear_wait_queue;
}

void pop_wait(wait_queue *WQ) {
    if (WQ->front_wait_queue == NULL) {
        exit(-1);
    } else if (WQ->length == 1) {
        // Wait Queue TCB 1개
        Thread *pop_thread = WQ->front_wait_queue;
        // 삭제한 Thread 설정 변경
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;

        // Wait queue 변경
        WQ->front_wait_queue = NULL;
        WQ->length = 0;
    } else {
        // 첫 번째 Thread 삭제
        Thread *pop_thread = WQ->front_wait_queue;
        WQ->front_wait_queue = pop_thread->pNext;
        WQ->front_wait_queue->pPrev = NULL;
        // 새로운 Thread 설정
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;
        // Queue 길이 설정
        WQ->length--;
    }

    // WaitQ 설정
    WaitQHead = WQ->front_wait_queue;
    WaitQTail = WQ->rear_wait_queue;
}

// Queue에 존재하는 모든 Thread의 tid 출력
void show_wait(wait_queue *WQ) {
    Thread *cursor;
    // Queue가 비었다면
    if (WQ->front_wait_queue == NULL) {
        printf("WAIT QUEUE EMPTY\n");
        return;
    }
    printf("WAIT Thread ID: ");
    // 스레도 tid 출력
    for (cursor = WQ->front_wait_queue; cursor != NULL;
         cursor = cursor->pNext) {
        printf(" %ld ", cursor->tid);
    }
    printf("\n");
}

// target tid가 있는 Thread 포인터를 반환 (NULL 실패)
Thread *find_wait(wait_queue *WQ, thread_t target_tid) {
    Thread *cursor = WQ->front_wait_queue;
    // Queue 처음부터 tid 검색
    while ((cursor != NULL) && (cursor->tid != target_tid)) {
        cursor = cursor->pNext;
    }
    if (cursor == NULL) {
        puts("tid no exist");
        return NULL;
    }
    return cursor;
}

void remove_thread_wait(wait_queue *WQ, Thread *remove_thread) {
    // 큐에 한개 존재하는 경우
    if (WQ->length == 1) {
        WQ->front_wait_queue = NULL;
        WQ->rear_wait_queue = NULL;
    }
    // 큐 처음에 존재
    else if (remove_thread->pPrev == NULL) {
        remove_thread->pNext->pPrev = NULL;
        WQ->front_wait_queue = remove_thread->pNext;
    }
    // 큐 마지막에 존재
    else if (remove_thread->pNext == NULL) {
        remove_thread->pPrev->pNext = NULL;
        WQ->rear_wait_queue = remove_thread->pPrev;
    }
    // 큐 중간에 위치하는 경우
    else {
        remove_thread->pPrev->pNext = remove_thread->pNext;
        remove_thread->pNext->pPrev = remove_thread->pPrev;
    }
    remove_thread->pPrev = NULL;
    remove_thread->pNext = NULL;
    WQ->length--;

    // WaitQ 설정
    WaitQHead = WQ->front_wait_queue;
    WaitQTail = WQ->rear_wait_queue;
}

// Zombie Queue 초기화
zombie_queue *init_zombie_queue() {
    // ready queue 생성
    zombie_queue *ZQ = (zombie_queue *)malloc(sizeof(zombie_queue));
    if (ZQ == NULL) { // 메모리 할당 실패
        return NULL;
    }

    ZQ->length = 0;
    ZQ->front_zombie_queue = NULL;
    ZQ->rear_zombie_queue = NULL;

    return ZQ;
}
// Zombie Queue 삽입
void push_zombie(zombie_queue *ZQ, Thread *thread_entry) {
    // 첫번째 노드
    if (ZQ->front_zombie_queue == NULL) {
        // Queue의 처음과 마지막 설정
        ZQ->front_zombie_queue = thread_entry;
        ZQ->rear_zombie_queue = thread_entry;
        // Thread의 이전과 이후 Thread 설정
        thread_entry->pPrev = NULL;
        thread_entry->pNext = NULL;
        // Queue 길이
        ZQ->length = 1;
    } else {
        // 마지막 Thread의 이후 노드 설정
        ZQ->rear_zombie_queue->pNext = thread_entry;
        // 삽입하는 Thread의 이전 노드 설정
        thread_entry->pPrev = ZQ->rear_zombie_queue;
        thread_entry->pNext = NULL;
        // Queue의 마지막에 Thread 삽입
        ZQ->rear_zombie_queue = thread_entry;
        ZQ->length++;
    }
}
// Zombie Queue 삭제
void pop_zombie(zombie_queue *ZQ) {
    if (ZQ->front_zombie_queue == NULL) { // error
        exit(-1);
    } else if (ZQ->length == 1) {
        // Ready Queue TCB 1개
        Thread *pop_thread = ZQ->front_zombie_queue;
        // 삭제한 Thread 설정 변경
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;

        // Ready queue 변경
        ZQ->front_zombie_queue = NULL;
        ZQ->length = 0;
    } else {
        // 첫 번째 Thread 삭제, Queue 헤드 업데이트
        Thread *pop_thread = ZQ->front_zombie_queue;
        ZQ->front_zombie_queue = pop_thread->pNext;
        ZQ->front_zombie_queue->pPrev = NULL;
        // 삭제한 Thread 설정 변경
        pop_thread->pPrev = NULL;
        pop_thread->pNext = NULL;
        // Queue 길이 설정
        ZQ->length--;
    }
}
// Zombie Queue 출력
void show_zombie(zombie_queue *ZQ) {
    Thread *cursor;
    // Queue가 비었다면
    if (ZQ->front_zombie_queue == NULL) {
        printf("ZOMBIE QUEUE EMPTY\n");
        return;
    }
    printf("ZOMBIE Thread ID:");
    // 스레도 tid 출력
    for (cursor = ZQ->front_zombie_queue; cursor != NULL;
         cursor = cursor->pNext) {
        printf("%ld ", cursor->tid);
    }
    printf("\n");
}
// 원하는 tid Zombie Queue에서 검색
Thread *find_zombie(zombie_queue *ZQ, thread_t target_tid) {
    Thread *cursor = ZQ->front_zombie_queue;
    // Queue 처음부터 tid 검색
    while ((cursor != NULL) && (cursor->tid != target_tid)) {
        cursor = cursor->pNext;
    }

    if (cursor == NULL) {
        puts("tid no exist");
        return NULL;
    }

    return cursor;
}
// 원하는 TCB Zombie Queue에서 삭제
void remove_thread_zombie(zombie_queue *ZQ, Thread *remove_thread) {
    // 큐에 한개 존재하는 경우
    if (ZQ->length == 1) {
        ZQ->front_zombie_queue = NULL;
        ZQ->rear_zombie_queue = NULL;
    }
    // 큐 처음에 존재
    else if (remove_thread->pPrev == NULL) {
        remove_thread->pNext->pPrev = NULL;
        ZQ->front_zombie_queue = remove_thread->pNext;
    }
    // 큐 마지막에 존재
    else if (remove_thread->pNext == NULL) {
        remove_thread->pPrev->pNext = NULL;
        ZQ->rear_zombie_queue = remove_thread->pPrev;
    }
    // 큐 중간에 위치하는 경우
    else {
        remove_thread->pPrev->pNext = remove_thread->pNext;
        remove_thread->pNext->pPrev = remove_thread->pPrev;
    }
    remove_thread->pPrev = NULL;
    remove_thread->pNext = NULL;
    ZQ->length--;
}
