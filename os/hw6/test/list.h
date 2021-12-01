#include <stdio.h>
#include <stdlib.h>

// 노드 구조체
typedef struct __node {
    int start_num;	    // 출발점 번호
    int time;			// 도착까지 남은 시간
	struct __node *prev;
	struct __node *next;
}Node;

// 리스트 구조체
typedef struct __list {
    Node *head;
    Node *tail;
    int cnt;
}List;

int __get_cnt(List *list);
void __add(int start_num, int time, List *list);
Node *__find(int start_num, List *list);
Node *__get(int idex, List *list);
Node *__get_random(List *list);
void __remove(Node *node, List *list);
void __delete_all(List *list);
void __print_all(List *list);

