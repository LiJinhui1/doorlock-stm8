#include<stdio.h>
#include<stdlib.h>
#include"stack.h"
void push(Stack L,int i) {
	Stack p = (Stack)malloc(sizeof(node_type));
	p->next=L->next;
	L->next = p;
	p->data = i;
}

void pop(Stack& L) {
	Stack p = L->next;
	L->next = p->next;
	free(p);
	p = NULL;
}

Stack InitStack() {
	Stack L= (Stack)malloc(sizeof(node_type));
	L->data = 0;
	L->next = NULL;
	printf("请输入栈的元素");//输入0结束输入
	int x;
	scanf_s("%d", &x);
	while (x) {
		push(L, x);
		scanf_s("%d", &x);
	}
	return L;
}

void OutputStack(Stack& L) {
	node_type* p = L->next;
	printf("\n栈的元素为：");
	while (p) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}

void Initdhstack(dhstack* L) {
	L->top1 = -1;
	L->top2 = MAXNUM;
}

void pushdh(dhstack* s, int which, int x) {
	if (s->top1 + 1 == s->top2) {
		printf("栈已满！停止输入！");
		return;
	}
	else {
		if (which == 1) {
			s->data[s->top1 + 1] = x;
			s->top1++;
		}
		else if (which == 2) {
			s->data[s->top2 - 1] = x;
			s->top2--;
		}
		else {
			printf("error!");
		}
	}
}

int popdh(dhstack * s, int which) {
	int result;
	if (which == 1) {
		if (s->top1 == -1) {
			printf("栈已为空，无法出栈！");
			return 0;
		}
		result = s->data[s->top1];
		s->top1--;
	}
	else if (which == 2) {
		if (s->top2 == MAXNUM){
			printf("栈已为空，无法出栈！");
			return 0;
		}
		result = s->data[s->top2];
		s->top2++;
	}
	else {
		printf("error");
	}
	return result;
}

void popall(dhstack* s) {
	int num1 = s->top1 + 1;
	int num2 = 6 - s->top2;
	int i = 0;
	printf("\n栈1的出栈顺序为");
	for (i = 0; i <= num1 - 1; i++) {
		printf("%d ", popdh(s, 1));
	}
	printf("\n栈2的出栈顺序为");
	for (i = 0; i <= num2 - 1; i++) {
		printf("%d ", popdh(s, 2));
	}
}




