#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) 
{
	int i, cnt, len1, len2;
	char ch, op;
	char exp[BUFFER_SIZE];
	char values[2][BUFFER_SIZE];
	char *ptr, *p1, *p2;
	double d1, d2, res;

	// 수식 입력
	i = 0, cnt = 0;
	while ((ch = getchar()) != '\n') {
		if (ch == '+' || ch == '-' || ch == '*' || ch == '%') {
			// 연산자 및 연산자 개수 저장
			op = ch;
			cnt++;
		}
		exp[i++] = ch;
	}
	exp[i] = '\0';

	// 수식에 연산자가 없는 경우
	if (cnt == 0) {
		fprintf(stderr, "error : %s\n", "연산자가 없습니다."); 
		return 1;
	}

	// 연산자가 2개 이상인 경우
	if (cnt > 1) {
		fprintf(stderr, "error : %s\n", "연산자의 개수가 2개 이상입니다."); 
		return 1;
	}

	ptr = strtok(exp, "+-*%");

	for (int i = 0; i < 2; i++) {
		strcpy(values[i], ptr);
		ptr = strtok(NULL, "+-*%");
	}

	d1 = strtof(values[0], &p1);
	d2 = strtof(values[1], &p2);
	len1 = strlen(values[0]);
	len2 = strlen(values[1]);

	// strtof() 는 두번째 인자에 숫자로 변경하지 못하는 문자열의 시작 위치가 저장됨
	if (*p1 != '\0' || *p2 != '\0') {
		fprintf(stderr, "error : %s\n", "피연산자가 숫자의 형태가 아닙니다."); 
		return 1;
	}

	// '10.', '11.' 과 같은 형태의 문자열 제외
	if (values[0][0] == '.' || values[1][0] == '.') {
		fprintf(stderr, "error : %s\n", "피연산자가 숫자의 형태가 아닙니다."); 
		return 1;
	}

	// '.10', '.11', 과 같은 형태의 문자열 제외
	if (values[0][len1 - 1] == '.' || values[1][len2 - 1] == '.') {
		fprintf(stderr, "error : %s\n", "피연산자가 숫자의 형태가 아닙니다."); 
		return 1;
	}

	if (op == '+') {
	} else if (op == '-') {
	} else if (op == '*') {
	} else if (op == '%') {
	}

	printf("%lf\n", res);

	return 0;
}
