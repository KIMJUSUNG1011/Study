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
int is_hit(int p_num);
int is_full(int frame_num);
void update_frame(int *frame);
void print_frame(int time, int page_fault, int *frame, int frame_num);
void print_format(const char *type, int frame_num, int ref_num, int *ref_string);
Node *get_victim_opt(int cur, int frame_num, int ref_num, int *frame, int *ref_string);
Node *get_victim_fifo();
Node *get_victim_lru();
Node *get_victim_second_chance();
void simulate(const char *type, int frame_num, int ref_num, int *ref_string);
