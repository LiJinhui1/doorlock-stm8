#pragma once
//循环队列
#define N 20
#define MAXNUM2 5
#include<stdio.h>
typedef struct
{
	int data[N];
	int front, rear;
}queue_type;
int Enqueue(queue_type* L, int x);
int Dequeue(queue_type* L, int*e);
void Outputqueue(queue_type* L);
void Initqueue(queue_type* L);
void aa(queue_type* q);

//队列扩展 ex3-4
typedef struct ExtQueue {
    int front;
    int rear;
    int flag;
    int data[MAXNUM2];
}extqueue;
void Initextqueue(extqueue* L);
int Enextqueue(extqueue* L, int x);
int Dextqueue(extqueue* L, int* e);