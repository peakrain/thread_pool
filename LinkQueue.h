#ifndef _queue_h
#define _queue_h

#include<stdlib.h>
#define true 1
#define false 0
typedef struct qnode{
	void *data;
	struct qnode *next;
}QNode;	
typedef struct{
	QNode *front;
	QNode *rear;
}LinkQueue;

typedef LinkQueue *queue_t;

queue_t Queue_create();
int Queue_enqueue(queue_t q,void *element);
void Queue_dequeue(queue_t q);
int Queue_length(queue_t q);
void *getFront(queue_t Q);
int isempty(queue_t q);
void Queue_destory(queue_t q);

#endif
