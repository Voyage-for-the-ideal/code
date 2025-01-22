//¿®∫≈≈‰∂‘ºÏ—È
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACKSIZE 100
#define OVERFLOW -1
#define ERROR -1
#define OK 1

typedef struct {
	char *base;
	char *top;
	int stacksize;
} SqStack;

void InitStack(SqStack *sqstack) {
	sqstack->base = (char *)malloc(STACKSIZE * sizeof(char));
	if (!sqstack->base)
		exit(OVERFLOW);
	sqstack->top = sqstack->base;
	sqstack->stacksize = STACKSIZE;
}

char pop(SqStack *sqstack) {
	char e;
	if (sqstack->top == sqstack->base)
		return ERROR;
	e = * sqstack->top;
	sqstack->top--;
	return e;
}

void Push(SqStack *sqstack, char e) {
	if (sqstack->top - sqstack->base >= sqstack->stacksize) {
		sqstack->base = (char *)realloc(sqstack->base, (sqstack->stacksize + STACKSIZE) * sizeof(char));
		if (!sqstack->base)
			exit(OVERFLOW);
		sqstack->top = sqstack->base + sqstack->stacksize;
		sqstack->stacksize += STACKSIZE;
	}
	sqstack->top++;
	*sqstack->top = e;
}

int main() {
	char buffer[STACKSIZE];
	int i = 0;
	char e = NULL;
	SqStack sqstack;
	InitStack(&sqstack);
	printf("«Î ‰»Î£∫");
	fgets(buffer, sizeof(buffer), stdin);//Èèà‚Ç¨Êæ∂Ê∞¨ÔΩûÈçêÔø?100Ê∂ìÔøΩ
	if (buffer[0] == '\n') {
		printf("Œ¥ºÏ≤‚µΩ ‰»Î");
		free(sqstack.base);
		return ERROR;

	} else if (buffer[0] == ']' || buffer[0] == '}' || buffer[0] == ')') {

		free(sqstack.base);
		return ERROR;
	}

	while (buffer[i] != '\n') {
		if (buffer[i] == '[' || buffer[i] == '{' || buffer[i] == '(') {
			Push(&sqstack, buffer[i]);
			i++;
		} else if (buffer[i] == ']' || buffer[i] == '}' || buffer[i] == ')') {
			e = pop(&sqstack);
			if ((buffer[i] == ']' && e == '[') || (buffer[i] == '}' && e == '{' ) || (buffer[i] == ')' && e == '(')) {
				i++;
			} else {
				free(sqstack.base);
				printf("ERROR");
				return ERROR;
			}
		}
		else i++;

	}
	printf("YES");
	free(sqstack.base);
	return 0;
}