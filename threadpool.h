#ifndef _threadpool_h
#define _threadpool_h

#include"LinkQueue.h"
#include<pthread.h>

struct task{
	void *(*routine)(void *arg);
	void *arg;
};
struct thread_pool{
	int thread_count;
	queue_t task;
	pthread_t *threads;
	pthread_mutex_t p_lock;
	pthread_cond_t p_ready;	
};

typedef struct thread_pool *thread_pool_t;
thread_pool_t thread_pool_create(int n);
void thread_pool_add_task(thread_pool_t pool,void *(*routine)(void *arg),void *arg);
#endif
