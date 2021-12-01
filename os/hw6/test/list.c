#include "list.h"

// 노드의 개수 리턴
int __get_cnt(List *list) {
	return list->cnt;
}

// 노드 추가
// 노드를 생성한 뒤 tail 에 노드를 연결
void __add(int start_num, int time, List *list) {

	Node *new = malloc(sizeof(Node));
	new->start_num = start_num;
	new->time = time;

    if (list->cnt == 0) {
        list->head = new;
        list->tail = new;
    } else {
        new->next = list->tail;
        list->tail->prev = new;
        list->tail = new;
    }

    (list->cnt)++;
}

// start_num 인 노드가 있는지 체크
// 성공 시 : 노드의 위치, 실패 시 : NULL
Node *__find(int start_num, List *list) {

	int i;
	Node *cur = list->tail;

	for (i = 0; i < list->cnt; i++) {
		if (cur->start_num == start_num) {
			return cur;
		}
		cur = cur->next;
	}

	return NULL;
}

// tail 을 기준으로 idx 의 위치에 있는 노드 리턴
Node *__get(int idx, List *list) {

	if (idx < 0 || idx >= __get_cnt(list)) {
		fprintf(stderr, "get error");
		return NULL;
	}

	int i;
	Node *cur = list->tail;

	for (i = 0; i < idx; i++) {
		cur = cur->next;
	}

	return cur;
}

// 리스트에서 랜덤하게 하나의 노드를 선택
Node *__get_random(List *list) {
    return NULL;
}

// target 노드 삭제
void __remove(Node *target, List *list) {

	if (list->cnt == 0 || target == NULL) {
		printf("remove error : list is empty\n");
		return;
	}

	(list->cnt)--;

	if (list->head == list->tail) {
		list->head = NULL;
		list->tail = NULL;
		free(target);
		return;
	}

	if (target == list->head) {
		list->head = target->prev;
		list->head->next = NULL;
		free(target);
		return;
	}

	if (target == list->tail) {
		list->tail = target->next;
		list->tail->prev = NULL;
		free(target);
		return;
	}

	target->prev->next = target->next;
	target->next->prev = target->prev;
	free(target);
}

// 리스트 삭제
void __delete_all(List *list) {

	int i;
	int __cnt = __get_cnt(list);
	Node *cur;

	for (i = 0; i < __cnt; i++) {
		cur = list->tail->next;
		free(list->tail);
		list->tail = cur;
	}

	list->head = NULL;
	list->tail = NULL;
	list->cnt = 0;
}

// 리스트 출력
void __print_all(List *list) {

	int i;
	Node *cur = list->tail;

	if (list->cnt == 0) {
		printf("print_all error : list is Empty\n");
		return;
	}

	for (i = 0; i < list->cnt; i++) {
		printf("(%d %d)", cur->start_num, cur->time);
		if (i == list->cnt - 1) {
			printf("\n");
			break;
		}
		printf("->");
		cur = cur->next;
	}
}

