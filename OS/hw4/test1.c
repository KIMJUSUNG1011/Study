#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define N_SIZE 128

void multiply();

int main(void)
{
	pid_t pid;
	int i, size, status;
	char fname[N_SIZE];

	for (i = 0; i < 21; i++) {
		if ((pid = fork()) == 0) {

			printf("%d : %d process begins\n", i, getpid());

			if (i >= 0 && i < 7) {
				setpriority(PRIO_PROCESS, 0, 10);
				size = 400;
			}
			else if (i >= 7 && i < 14) {
				setpriority(PRIO_PROCESS, 0, 0);
				size = 500;
			}
			else {
				setpriority(PRIO_PROCESS, 0, -15);
				size = 700;
			}

			multiply(size);

			exit(0);
		} 
		else if (pid < 0) {
			fprintf(stderr, "fork error\n");
			exit(1);
		}
	}

	for (i = 0; i < 20; i++) {
		pid = wait(&status);
		printf("%d process ends\n", pid);
	}

	exit(0);
}

void multiply(int size)
{
	long a[size][size], b[size][size], c[size][size];
	long i, j, k, in;

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

