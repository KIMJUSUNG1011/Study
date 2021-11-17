#include "util.h"

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

	simulate("OPT", frame_num, ref_num, ref_string);
	simulate("FIFO", frame_num, ref_num, ref_string);
	simulate("LRU", frame_num, ref_num, ref_string);
	simulate("SECOND-CHANCE", frame_num, ref_num, ref_string);

	exit(0);
}

