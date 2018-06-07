#ifndef _threadpool_h
#define _threadpool_h

#include"LinkQueue.h"
#include<pthread.h>

struct task_info{
	int total;
	int processing;
	int waiting;
	int finished;
};
struct thread_info{
	int total;
	int used;
	int free;
};
struct task{
	void *(*routine)(void *arg);
	void *arg;
};
struct thread_pool{
	queue_t task;
	struct task_info task_info;
	struct thread_info thread_info;
	pthread_t info_id;
	pthread_t *threads;
	pthread_mutex_t task_lock;
	pthread_rwlock_t rw_lock;
	pthread_cond_t task_ready;	
};

typedef struct thread_pool *thread_pool_t;
thread_pool_t thread_pool_create(int n);
void thread_pool_add_task(thread_pool_t pool,void *(*routine)(void *arg),void *arg);
void thread_pool_destroy(thread_pool_t pool);
#endif
