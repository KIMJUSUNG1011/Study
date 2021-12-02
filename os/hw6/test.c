#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

void *routine(void *);

List *ready;            // 출발 대기 상태인 차량들
List *process;          // 차로를 점유하고 있는 차량들
int select_start_num;   // 선택된 차량의 출발점 번호

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    int i, j, tick;
    Node *target;
    pthread_t tids[4];
    int order[] = {4, 4, 3, 4, 1, 2, 2, 3, 3, 3};
    int nums[] = {1, 2, 3, 4};
    int vehicles[4];

    // ready, process 리스트 초기화
    ready = malloc(sizeof(List));
    process = malloc(sizeof(List));

    // 각 출발점에서 출발해 도착한 차량의 수를 저장
    memset(vehicles, 0, sizeof(vehicles));

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

    // 출발 대기인 차량 중 출발할 차량을 랜덤하게 선택하기 위한 난수 생성
    srand((unsigned int)time(NULL));

    sleep(1);

    i = 0;
    tick = 0;

    while (1) {

        // 해당 순서의 차량을 ready 리스트에 삽입
        if (i < 10) {
            __add(order[i++], -1, ready);
        }

        // ready 리스트에서 랜덤하게 하나의 차량을 선택
        target = __get_random(ready);

        // 제어권이 해당 출발점 스레드로 넘어감
        if (target != NULL) {
            select_start_num = target->start_num;
        }

        sleep(1);

        // 제어권이 다시 main 스레드로 넘어옴
        pthread_mutex_lock(&mutex);

        // process 리스트를 업데이트(time = time - 1) 시킴으로서
        // 도로에 있는 모든 차량들을 진행시킴
        for (j = 0; j < __get_cnt(process); j++) {
            target = __get(j, process);
            (target->time)--;
        }

        // 진행사항 출력
        printf("tick : %d", ++tick);
        printf("\n===================================\n");
        printf("Passed Vehicle\nCar ");

        // 교차로를 빠져나간 차량 출력
        for (j = 0; j < __get_cnt(process); j++) {

            target = __get(j, process);

            if (target->time == 0) {

                printf("%d ", target->start_num);

                // 도달한 차량의 수 업데이트
                vehicles[target->start_num - 1]++;

                // 교차로를 빠져나갔으므로 process 리스트에서 삭제
                __remove(target, process);
            }
        }

        // 출발 대기중인 차량 출력
        printf("\nWaiting Vehicle\nCar ");
        for (j = 0; j < __get_cnt(ready); j++) {

            target = __get(j, ready);

            printf("%d ", target->start_num);
        }
        printf("\n===================================\n");

        pthread_mutex_unlock(&mutex);

        // 진행이 완료되면 루프 탈출
        if (__get_cnt(ready) == 0 && __get_cnt(process) == 0) {
            printf("tick : %d", ++tick);
            printf("\n===================================\n");
            printf("Passed Vehicle\nCar");
            printf("\nWaiting Vehicle\nCar");
            printf("\n===================================\n");
            break;
        }
    }

    printf("Number of vehicles passed from each start point\n");
    for (i = 0; i < 4; i++) {
        printf("P%d : %d times\n", i + 1, vehicles[i]);
    }
    printf("Total time : %d ticks\n", tick);

    exit(0);
}

void *routine(void *arg)
{
    // 출발점 번호
    int num;

    num = *((int *)arg);

    while (1) {

        // num 번 출발점의 차량에 대한 제어권을 가져옴
        if (num == select_start_num) {

            int flag;

            pthread_mutex_lock(&mutex);
            // printf("%d thread is selected\n", num);

            // num 번 출발점의 차량이 출발할 수 있는 지 확인
            flag = (__find(num, process) != NULL) 
                || (__find(num % 4 + 1, process) != NULL) 
                || (__find((num + 2) % 4 + 1, process) != NULL);

            // 도로에 자신의 출발점이나 인접한 출발점에서 출발한 차량이 없을 경우
            // 즉, num 번 출발점의 차량이 출발 가능한 경우
            if (!flag) {

                Node *target;

                // 해당 차량을 ready 리스트에서 삭제
                target = __find(num, ready);
                __remove(target, ready);

                // 해당 차량을 process 리스트에 삽입
                __add(num, 2, process);
            }

            select_start_num = 0;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

