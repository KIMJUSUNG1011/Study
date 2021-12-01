#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_loop1(void *);
void *ssu_loop2(void *);

pthread_mutex_t mutex1 = pthread_mutex_initializer;
pthread_mutex_t mutex2 = pthread_mutex_initializer;
int shared_value;

int main(void)
{
    pthread_t tid1, tid2;
    int status;

    if (pthread_create(&tid1, NULL, ssu_loop1, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, ssu_loop2, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(1);
    }

    pthread_join(tid1, (void *)&status);
    pthread_join(tid2, (void *)&status);

    status = pthread_mutex_destroy(&mutex1);
    exit(0);
}

void *ssu_loop1(void *arg)
{
    int i;

    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex1);
        printf("loop1 : %d\n", shared_value);
        shared_value++;
        pthread_mutex_unlock(&mutex1);
        sleep(1);
    }

    return NULL;
}

void *ssu_loop2(void *arg)
{
    int i;

    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex1);
        printf("loop2 : %d\n", shared_value);
        shared_value++;
        pthread_mutex_unlock(&mutex1);
        sleep(1);
    }

    return NULL;
e
