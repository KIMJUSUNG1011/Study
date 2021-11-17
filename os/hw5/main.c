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
void opt(int frame_num, int ref_num, int *ref_string);
int is_hit(int p_num);
int is_full(int frame_num);
void update_frame(int *frame);
void print_frame(int time, int page_fault, int *frame, int frame_num);
Node *get_victim(int cur, int frame_num, int ref_num, int *frame, int *ref_string);

int main(void) {

	int fd, frame_num, ref_num;
	char fname[F_NAME_SIZE];
	int ref_string[50];

	printf("Input file name : ");
	scanf("%[^\n]s", fname);
	printf("\n");

	if ((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

	frame_num = get_frame_num(fd);
	ref_num = get_ref_string(fd, ref_string);

	opt(frame_num, ref_num, ref_string);

	exit(0);
}

// 1. 큐가 가득차 있지 않은 상황
//	 - 들어갈 원소의 프레임 번호 : 큐에 들어 있는 원소의 개수 + 1

// 2. 큐가 가득차 있는 상황
//   - 교체 발생

void opt(int frame_num, int ref_num, int *ref_string) {

	int i, hit_num = 0;
	int *frame = (int*)malloc(sizeof(int) * frame_num);

	memset(frame, -1, sizeof(int) * frame_num);

	// 출력 형식
	printf("Used method : OPT\n");
	printf("page reference string :");
	for (i = 0; i < ref_num; i++) {
		printf("%6d", ref_string[i]);
	} printf("\n\n");
	printf("%10s%5s", "frame", " "); 
	for (i = 0; i < frame_num; i++) {
		printf("%-7d", i + 1);
	} 
	printf("%-7s\n", "page fault");
	printf("time\n");

	for (i = 0; i < ref_num; i++) {

		int p_num = ref_string[i];
		int f_num = 0;
		Node *target;

		// hit 여부 체크
		if (!is_hit(p_num)) {
			hit_num++;
			print_frame(i + 1, 0, frame, frame_num);
			continue;
		}

		if (!is_full(frame_num)) {	// 교체 발생

			// 교체될 노드를 opt 방식으로 탐색
			if ((target = get_victim(i, frame_num, ref_num, frame, ref_string)) == NULL) {
				fprintf(stderr, "get_victim error\n");
				return;
			}

			// 교체될 노드의 f_num 을 저장
			f_num = target->f_num;

			// 교체될 노드를 리스트에서 삭제
			__remove(target);

			// 새로운 노드를 리스트에 저장
			__add(p_num, f_num, -1);

		} else {					// 교체가 발생하지 않음
			f_num = __get_cnt() + 1;
			__add(p_num, f_num, -1);
		}

		update_frame(frame);
		print_frame(i + 1, 1, frame, frame_num);
	}

	printf("Number of page faults : %d times\n", ref_num - hit_num);
}

// 교체될 노드를 opt 방식으로 탐색
Node *get_victim(int cur, int frame_num, int ref_num, int *frame, int *ref_string) {

	int i, j, len, pos, max = -1;

	// ref_string 의 현재 위치에서 가장 멀리 떨어진,
	// 즉 가장 오랫동안 사용되지 않을 p_num 을 탐색
	for (i = 0; i < frame_num; i++) {

		len = 0;

		for (j = cur; j < ref_num; j++) {
			if (ref_string[j] == frame[i]) {
				break;
			}
			len++;
		}

		if (max < len) {
			max = len;
			pos = i;
		}
	}

	return __find(frame[pos]);
}

// p_num 프로세스 hit 여부 확인 
// hit 시 : 0, miss 시 : 1 리턴
int is_hit(int p_num) {

	if (__find(p_num) != NULL) {
		return 0;
	} else {
		return 1;
	}
}

// 큐가 가득차 있으면 0을, 아니면 1을 리턴
int is_full(int frame_num) {

	if (frame_num == __get_cnt()) {
		return 0;
	} else {
		return 1;
	}
}

// 프레임 업데이트
void update_frame(int *frame) {

	int i ;

	for (i = 0; i < __get_cnt(); i++) {
		Node *cur = __get(i);
		frame[cur->f_num - 1] = cur->p_num;
	}
}

// 프레임 출력
void print_frame(int time, int page_fault, int *frame, int frame_num) {

	int i;

	printf("%-15d", time);

	for (i = 0; i < frame_num; i++) {
		if (frame[i] != -1) {
			printf("%-7d", frame[i]);
		} else {
			printf("%-7s", " ");
		}
	} 

	if (page_fault) {
		printf("%-7s", "F");
	} printf("\n");
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

