#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "list.h"

void *routine(void *);

List *ready;                                        // 출발 대기 상태인 차량들
List *process;                                      // 차로를 점유하고 있는 차량들
int select_start_num;                               // 선택된 차량의 출발점 번호
int is_end;                                         // 출발점 스레드들의 종료를 관리하는 플래그

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // mutex 초기화

int main(void)
{
    int i, j, tick, status, num_of_vehicles;
    int *order;                                     // 출발점의 순서를 저장
    int vehicles[4] = {0,};                         // 각 출발점에서 출발해 도착한 차량의 수를 저장
    int thread_index[4] = {0,};                     // 각 스레드의 인덱스 번호를 저장
    pthread_t tids[4];
    Node *target;

    // 난수 생성
    srand((unsigned int)time(NULL));

    // 입력 양식
    printf("Total number of vehicles : ");
    scanf("%d", &num_of_vehicles);
    order = malloc(sizeof(int) * num_of_vehicles);

    printf("Start point : ");
    for (i = 0; i < num_of_vehicles; i++) {
        j = (int)rand() % 4 + 1;
        order[i] = j;
        printf("%d ", j);
    } printf("\n");
 
    // ready, process 리스트 생성
    ready = malloc(sizeof(List));
    process = malloc(sizeof(List));

    // 스레드들의 종료 관리
    is_end = 0;

    // 4개의 출발점 스레드 생성
    for (i = 0; i < 4; i++) {
        thread_index[i] = i + 1;
        if (pthread_create(&tids[i], NULL, routine, (void *)&thread_index[i]) != 0) {
            fprintf(stderr, "pthread_create error\n");
            exit(1);
        }
    }

    i = 0;
    tick = 0;

    while (1) {

        // 해당 순서의 차량을 ready 리스트에 삽입
        if (i < num_of_vehicles) {
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

            // 출발점 스레드들을 종료시킴
            is_end = 1;

            break;
        }
    }

    // 최종 결과 출력
    printf("Number of vehicles passed from each start point\n");
    for (i = 0; i < 4; i++) {
        printf("P%d : %d times\n", i + 1, vehicles[i]);
    }
    printf("Total time : %d ticks\n", tick);

    // 자원 해제
    __delete_all(ready);
    __delete_all(process);
    free(ready);
    free(process);
    free(order);

    // mutex 해제
    status = pthread_mutex_destroy(&mutex);

    // 출발점 스레드 종료 대기
    for (i = 0; i < 4; i++) {
        pthread_join(tids[i], (void *)&status);
    }

    exit(0);
}

// 출발점 스레드 루틴
void *routine(void *arg)
{
    // 출발점 번호
    int num;

    num = *((int *)arg);

    while (!is_end) {

        // num 번 출발점의 차량에 대한 제어권을 가져옴
        if (num == select_start_num) {

            int flag;
            Node *target;

            pthread_mutex_lock(&mutex);

            // num 번 출발점의 차량이 출발할 수 있는 지 확인
            flag = (__find(num, process) != NULL) 
                || (__find(num % 4 + 1, process) != NULL) 
                || (__find((num + 2) % 4 + 1, process) != NULL);

            // 도로에 자신의 출발점이나 인접한 출발점에서 출발한 차량이 없을 경우
            // 즉, num 번 출발점의 차량이 출발 가능한 경우
            if (!flag) {

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

    pthread_exit(NULL);

    return NULL;
}

