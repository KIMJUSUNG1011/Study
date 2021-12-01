#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "list.h"

void *routine(void *);

List *ready;    // 출발 대기 상태인 차량들
List *process;  // 차로를 점유하고 있는 차량들
int choice_num; // 선택된 차량의 출발점 번호

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    int i;
    pthread_t tids[4];
    int order[5] = {4, 4, 3, 1, 2};
    int nums[] = {1, 2, 3, 4};

    // ready, process 리스트 초기화
    ready = malloc(sizeof(List));
    process = malloc(sizeof(List));

    // 1번 출발점 스레드 생성
    if (pthread_create(&tids[0], NULL, routine, (void *)&nums[0]) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    // 2번 출발점 스레드 생성
    if (pthread_create(&tids[1], NULL, routine, (void *)&nums[1]) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    // 3번 출발점 스레드 생성
    if (pthread_create(&tids[2], NULL, routine, (void *)&nums[2]) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    // 4번 출발점 스레드 생성
    if (pthread_create(&tids[3], NULL, routine, (void *)&nums[3]) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    sleep(1);

    for (i = 0; i < 5; i++) {

        // 해당 순서의 차량을 ready 리스트에 삽입
        __add(order[i], -1, ready);

        // ready 리스트에서 랜덤하게 하나의 차량을 선택
        // choice_num = __get_random(ready);

        sleep(1);

        pthread_mutex_lock(&mutex);
        printf("main worked\n");
        pthread_mutex_unlock(&mutex);
    }

    exit(0);
}

void *routine(void *arg)
{
    int num;

    num = *((int *)arg);

    while (1) {

        if (num == choice_num) {
            pthread_mutex_lock(&mutex);
            printf("%d thread is selected\n", num);

            // num 번 출발점의 차량이 출발할 수 있는 지 확인

            choice_num = 0;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

