#include<stdio.h>
#include<stdlib.h>
#include"queue.h"
//ѭ������
int Enqueue(queue_type* L, int x) {
	if (L->front == (L->rear + 1) % N) {
		printf("����������\n");
		return 0;
	}
	else {
		L->data[L->rear] = x;
		L->rear = (L->rear + 1) % N;
		return 1;
	}
}

int Dequeue(queue_type* L, int* e) {
	if (L->front == L->rear) {
		printf("�����ѿգ�\n");
		return 0;
	}
	else {
		*e = L->data[L->front];
		L->front = (L->front + 1) % N;
		return 1;
	}
}

void aa(queue_type* q) {
	int x;
	int* zx = &x;
	int i;
	int len = (q->rear - q->front + N) % N;
	for (i = 0; i <= len - 1; i++) {
		Dequeue(q, zx);
		if (*zx > 0) {
			Enqueue(q, *zx);
		}
	}
}

void Initqueue(queue_type* L) {
	L->front = 0;
	L->rear = 0;
	printf("�������Ԫ��");
	int x;
	scanf_s("%d", &x);//����0��������
	while (x != 0) {
		if (Enqueue(L, x)) {
			scanf_s("%d", &x);
		}
		else {
			break;
		}
	}
}

void Outputqueue(queue_type* L) {
	int len = (L->rear - L->front + N) % N;
	int i;
	for (i = L->front; i <= len - 1+L->front; i++) {//i=0��Ϊi=L->front
		printf("%d ", L->data[i]);
	}
}

void Initextqueue(extqueue* L) {
	L->front = 0;
	L->rear = 0;
	L->flag = 0;
	printf("�������Ԫ��");
	int x;
	scanf_s("%d", &x);//����0��������
	while (x != 0){
		if (Enextqueue(L, x)) {
			scanf_s("%d", &x);
		}
		else {
			break;
		}
	}
}

int Enextqueue(extqueue* L,int x) {
	if (L->front == L->rear&&L->flag==1) {
		printf("����������\n");
		return 0;
	}
	else if(L->front == (L->rear + 1) % N) {
		L->data[L->rear] = x;
		L->rear = (L->rear + 1) % N;
		L->flag = 1;
		return 1;
	}
	else {
		L->data[L->rear] = x;
		L->rear = (L->rear + 1) % N;
		L->flag = 1;
		return 1;
	}
}

int Dextqueue(extqueue* L, int* e) {
	if (L->front == L->rear && L->flag == 0) {
		printf("�����ѿգ�\n");
		return 0;
	}
	else if (L->front != L->rear) {
		*e = L->data[L->front];
		L->front = (L->front + 1) % N;
		if (L->front == L->rear) {
			L->flag = 0;
		}
		return 1;
	}
}