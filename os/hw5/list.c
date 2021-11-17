#include "list.h"

// 노드의 개수 리턴
int __get_cnt() {
	return cnt;
}

// 노드를 생성한 뒤 tail 에 노드를 연결
void __add(int p_num, int f_num, int hit_label) {

	Node *new = malloc(sizeof(Node));
	new->p_num = p_num;
	new->f_num = f_num;
	new->hit_label = hit_label;

    if (cnt == 0) {
        head = new;
        tail = new;
    } else {
        new->next = tail;
        tail->prev = new;
        tail = new;
    }

    cnt++;
}

// p_num 인 노드가 있는지 체크
// 성공 시 : 노드의 위치, 실패 시 : NULL
Node *__find(int p_num) {

	int i;
	Node *cur = tail;

	for (i = 0; i < cnt; i++) {
		if (cur->p_num == p_num) {
			return cur;
		}
		cur = cur->next;
	}

	return NULL;
}

// target 노드 삭제
void __remove(Node *target) {

	if (cnt == 0 || target == NULL) {
		printf("remove error : list is empty\n");
		return;
	}

	cnt--;

	if (head == tail) {
		head = NULL;
		tail = NULL;
		free(target);
		return;
	}

	if (target == head) {
		head = target->prev;
		head->next = NULL;
		free(target);
		return;
	}

	if (target == tail) {
		tail = target->next;
		tail->prev = NULL;
		free(target);
		return;
	}

	target->prev->next = target->next;
	target->next->prev = target->prev;
	free(target);
}

// tail 을 기준으로 idx 의 위치에 있는 노드 리턴
Node *__get(int idx) {

	if (idx < 0 || idx >= __get_cnt()) {
		fprintf(stderr, "get error");
		return NULL;
	}

	int i;
	Node *cur = tail;

	for (i = 0; i < idx; i++) {
		cur = cur->next;
	}

	return cur;
}

// 리스트 삭제
void __delete_all() {

	int i;
	int __cnt = __get_cnt();
	Node *cur;

	for (i = 0; i < __cnt; i++) {
		cur = tail->next;
		free(tail);
		tail = cur;
	}

	head = NULL;
	tail = NULL;
	cnt = 0;
}

// 리스트 출력
void __print_all() {

	int i;
	Node *cur = tail;

	if (cnt == 0) {
		printf("print_all error : list is Empty\n");
		return;
	}

	for (i = 0; i < cnt; i++) {
		printf("(%d %d %d)", cur->p_num, cur->f_num, cur->hit_label);
		if (i == cnt - 1) {
			printf("\n");
			break;
		}
		printf("->");
		cur = cur->next;
	}
}

