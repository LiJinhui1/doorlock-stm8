#pragma once
#define MAXNUM 6
typedef struct node
{
	int  data;
	struct node* next;
}node_type;
typedef node_type* Stack;
typedef int ElemType;

void push(Stack L, int i);
void pop(Stack& L);
Stack InitStack();
void OutputStack(Stack& L);


typedef struct dhstack {
    int top1;
    int top2;
    int data[MAXNUM];
}dhstack;
void Initdhstack(dhstack* L);
void pushdh(dhstack* s, int which, int x);
int popdh(dhstack* s, int which);
void popall(dhstack* s);
