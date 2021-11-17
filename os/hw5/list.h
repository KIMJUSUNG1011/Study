#include <stdio.h>
#include <stdlib.h>

// 노드 구조체
typedef struct __node {
	int p_num;	// process 번호
	int f_num;	// frame 번호
	int hit_label;
	struct __node *prev;
	struct __node *next;
}Node;

// 리스트 정보
Node *head;
Node *tail;
int cnt;

int __get_cnt();
void __print_all();
void __add(int p_num, int f_num, int hit_label);
void __poll(); 
Node *__find(int p_num);
void __remove(Node *node);
void __update(Node *node, int p_num, int f_num, int hit_label);
Node *__get(int idx);
