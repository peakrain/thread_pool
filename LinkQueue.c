#include"LinkQueue.h"
#include<malloc.h>

queue_t Queue_create()
{
	queue_t Q=(queue_t)malloc(sizeof(LinkQueue));
	if(!Q)
		return NULL;
	Q->front=Q->rear=(QNode *)malloc(sizeof(QNode));
	Q->front->next=NULL;
	return Q;
}

int Queue_enqueue(queue_t Q,void *element)
{
	QNode *p=(QNode*)malloc(sizeof(QNode));
	if(!p)
		return false;
	p->data=element;
	p->next=NULL;
	Q->rear->next=p;
	Q->rear=p;
	return 0;		
}

void *getFront(queue_t Q)
{
	void *data=(void *)malloc(sizeof(void));
	data=Q->front->next->data;
	return data;
}
void Queue_dequeue(queue_t Q)
{
	if(Q->front==Q->rear)
		return;
	QNode *p=Q->front->next;
	Q->front->next=p->next;
	if(Q->rear==p)
		Q->rear=Q->front;
	free(p);
}

int Queue_length(queue_t Q)
{
	int size=0;
	QNode *p=(QNode*)malloc(sizeof(QNode));
	p=Q->front->next;
	while(p)
	{
		size++;
		p=p->next;
	}
	free(p);
	return size;
}
	
int isempty(queue_t Q)
{
	if(Q->rear==Q->front)
		return true;
	else
		return false;
}

void Queue_destory(queue_t Q)
{
	free(Q);
}
