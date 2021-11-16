#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include "list.h"

#define F_NAME_SIZE 1024
#define BUFFER_SIZE 128

int get_frame_num(int fd);
int get_ref_string(int fd, int *ref_string);

int main(void) {

	int fd, frame_num, ref_num;
	char fname[F_NAME_SIZE];
	int ref_string[50];

	printf("Input file name : ");
	scanf("%[^\n]s", fname);

	if ((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

	frame_num = get_frame_num(fd);
	ref_num = get_ref_string(fd, ref_string);

	printf("frame_num : %d\n", frame_num);
	printf("ref_num : %d\n", ref_num);

	for (int i = 0; i < ref_num; i++) {
		printf("%d ", ref_string[i]);
	} printf("\n");

	exit(0);
}

// page reference string 을 읽어들인 뒤 배열에 저장
// 리턴 값 : 레퍼런스의 개수
int get_ref_string(int fd, int *ref_string) {

	int idx, ref_num;
	char ch;
	char buf[BUFFER_SIZE];

	memset(buf, 0, sizeof(buf));
	idx = 0;
	ref_num = 0;

	while (1) {
		if (read(fd, &ch, 1) > 0) {
			if (ch == '\n') {
				break;
			}
			if (ch == ' ') {
				ref_string[ref_num++] = atoi(buf);
				memset(buf, 0, sizeof(buf));
				idx = 0;
			} else {
				buf[idx++] = ch;
			}
		}
	}

	ref_string[ref_num++] = atoi(buf);

	return ref_num;
}

// 입력파일에서 page frame 수를 추출
int get_frame_num(int fd) {

	int idx;
	char ch;
	char buf[BUFFER_SIZE];

	memset(buf, 0, sizeof(buf));
	idx = 0;

	while (1) {
		if (read(fd, &ch, 1) > 0) {
			if (ch == '\n') {
				break;
			} 
			buf[idx++] = ch;
		}
	}

	return atoi(buf);
}

