#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

void multiply();

int main(void)
{
	pid_t pid;
	int i, nice, status;

	for (i = 0; i < 21; i++) {

		if ((pid = fork()) == 0) {

			printf("%-3d : %d process begins\n", i, getpid());

			nice = getpriority(PRIO_PROCESS, 0);

			// 기본 nice 값 출력
			printf("%d's nice : %d\n", getpid(), nice);

			//  작업 수행
			multiply(400);

			exit(0);
		} 
		else if (pid < 0) {
			fprintf(stderr, "fork error\n");
			exit(1);
		}
	}

	for (i = 0; i < 21; i++) {
		pid = wait(&status);
		printf("%d process ends\n", pid);
	}

	printf("---------- All processes end ----------\n");

	exit(0);
}

void multiply(int size)
{
	int a[size][size], b[size][size], c[size][size];
	int i, j, k;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			a[i][j] = 1;
			b[i][j] = 1;
		}
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

