#include "Init.h"
#include "Thread.h"
#include "list.h"

#include <stdlib.h>

pthread_mutex_t mutex;
pthread_mutex_t zombie_mutex;

ready_queue *Ready_Queue;

wait_queue *Wait_Queue;

zombie_queue *Zombie_Queue;

void Init(void) {
    // mutex 초기화
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&zombie_mutex, NULL);

    // queue 초기화
    Ready_Queue = init_ready_queue();

    // wait 초기화
    Wait_Queue = init_wait_queue();

    // zombie 초기화
    Zombie_Queue = init_zombie_queue();
}
