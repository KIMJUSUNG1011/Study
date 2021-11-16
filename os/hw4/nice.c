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
	int i, size, status;

	for (i = 0; i < 21; i++) {

		if ((pid = fork()) == 0) {

			printf("%-3d: %d process begins\n", i, getpid());

			if (i >= 0 && i < 7) {
				// 높은 nice 값을 주고, 일을 조금 시킴
				setpriority(PRIO_PROCESS, 0, 10);
				size = 300;
			}
			else if (i >= 7 && i < 14) {
				// 중간 nice 값을 주고, 일을 적당히 시킴
				setpriority(PRIO_PROCESS, 0, 0);
				size = 400;
			}
			else {
				// 낮은 nice 값을 주고, 일을 많이 시킴
				setpriority(PRIO_PROCESS, 0, -10);
				size = 500;
			}

			// 작업 수행
			multiply(size);

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

// 행렬 곱셈 연산
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

