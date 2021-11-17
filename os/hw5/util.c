#include "util.h"

// 시뮬레이션 함수
void simulate(const char *type, int frame_num, int ref_num, int *ref_string) {

	int i, hit_num = 0;
	int *frame = (int*)malloc(sizeof(int) * frame_num);

	memset(frame, -1, sizeof(int) * frame_num);

	// 출력 양식
	print_format(type, frame_num, ref_num, ref_string);

	for (i = 0; i < ref_num; i++) {

		int p_num = ref_string[i];
		int f_num = 0;
		Node *target;

		// hit 인 경우
		if (is_hit(p_num)) {
			hit_num++;
			print_frame(i + 1, 0, frame, frame_num);

			// LRU 방식의 경우 hit 인 노드를 삭제한 뒤 큐의 맨 뒤에 다시 삽입
			if (!strcmp(type, "LRU")) {
				target = __find(p_num);
				__add(target->p_num, target->f_num, target->hit_label);
				__remove(target);
			}

			// SECOND-CHANCE 방식의 경우 hit 인 노드의 hit_label 을 1 로 세팅
			if (!strcmp(type, "SECOND-CHANCE")) {
				target = __find(p_num);
				target->hit_label = 1;
			}

			continue;
		}

		// 교체 발생
		if (is_full(frame_num)) {	

			// 교체 기법에 따라 교체할 노드를 선택
			if (!strcmp(type, "OPT")) {
				target = get_victim_opt(i, frame_num, ref_num, frame, ref_string);
			} 

			if (!strcmp(type, "FIFO")) {
				target = get_victim_fifo();
			}

			if (!strcmp(type, "LRU")) {
				target = get_victim_lru();
			}

			if (!strcmp(type, "SECOND-CHANCE")) {
				target = get_victim_second_chance();
			}

			// 교체될 노드의 f_num 을 저장
			f_num = target->f_num;

			// 교체될 노드를 리스트에서 삭제
			__remove(target);

			// 새로운 노드를 리스트에 저장
			__add(p_num, f_num, 0);

		} else {	// 교체가 발생하지 않음
			f_num = __get_cnt() + 1;
			__add(p_num, f_num, 0);
		}

		update_frame(frame);
		print_frame(i + 1, 1, frame, frame_num);
	}

	// 프레임 삭제
	free(frame);

	// 리스트 삭제
	__delete_all();

	printf("Number of page faults : %d times\n\n", ref_num - hit_num);
}

// 출력 형식
void print_format(const char *type, int frame_num, int ref_num, int *ref_string) {

	int i;

	printf("Used method : %s\n", type);
	printf("page reference string :");
	for (i = 0; i < ref_num; i++) {
		printf("%4d", ref_string[i]);
	} printf("\n\n");
	printf("%10s%5s", "frame", " "); 
	for (i = 0; i < frame_num; i++) {
		printf("%-7d", i + 1);
	} 
	printf("%-7s\n", "page fault");
	printf("time\n");
}

// 교체될 노드를 opt 방식으로 선택
Node *get_victim_opt(int cur, int frame_num, int ref_num, int *frame, int *ref_string) {

	int i, j, len, pos, max = -1;

	// ref_string 의 현재 위치에서 가장 멀리 떨어진,
	// 즉 가장 오랫동안 사용되지 않을 p_num 을 선택
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

// 교체될 노드를 fifo 방식으로 선택
Node *get_victim_fifo() {
	return head;
}

// 교체될 노드를 lru 방식으로 선택
Node *get_victim_lru() {
	return head;
}

// 교체될 노드를 second-chance 방식으로 선택
Node *get_victim_second_chance() {

	int i;
	Node *cur = head;
	Node *tmp;

	for (i = 0; i < __get_cnt(); i++) {

		// hit_label 이 0 이므로 해당 노드가 교체됨
		if (cur->hit_label == 0) {
			return cur;
		} 					

		// hit_label 이 1 이므로 한번의 기회를 더 줌
		// 기회를 한번 더 받은 노드는 큐의 맨 뒤로 이동
		tmp = cur->prev;
		__add(cur->p_num, cur->f_num, 0);
		__remove(cur);
		cur = tmp;
	}

	// 모든 프레임의 hit_label 이 1 인 경우 FIFO 방식을 따름
	return head;
}

// p_num 프로세스 hit 여부 확인 
// hit 시 : 1, miss 시 : 0 리턴
int is_hit(int p_num) {

	if (__find(p_num) != NULL) {
		return 1;
	} else {
		return 0;
	}
}

// 큐가 가득차 있으면 1 을, 아니면 0 을 리턴
int is_full(int frame_num) {

	if (frame_num == __get_cnt()) {
		return 1;
	} else {
		return 0;
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

