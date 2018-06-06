#include"threadpool.h"
#include<malloc.h>

void *thread_function(void *arg)
{
	thread_pool_t pool=(thread_pool_t)arg;
	while(1)
	{
		pthread_mutex_lock(&pool->p_lock);
		while(isempty(pool->task))
			pthread_cond_wait(&pool->p_ready,&pool->p_lock);
		struct task *t=getFront(pool->task);
		Queue_dequeue(pool->task);
		(*(t->routine))(t->arg);
		pthread_mutex_unlock(&pool->p_lock);
		free(t);
	}

}

thread_pool_t thread_pool_create(int n)
{
	int i;
	thread_pool_t pool=NULL;
	pool=(thread_pool_t)malloc(sizeof(struct thread_pool));
	pool->thread_count=n;
	pool->task=Queue_create();
	pool->threads=(pthread_t *)malloc(sizeof(pthread_t)*n);
	pthread_mutex_init(&pool->p_lock,NULL);
	pthread_cond_init(&pool->p_ready,NULL);
	for(i=0;i<n;i++)
		pthread_create(pool->threads+i,NULL,thread_function,pool);
	return pool;
}

void thread_pool_add_task(thread_pool_t pool,void *(*routine)(void *arg),void *arg)
{
	pthread_mutex_lock(&pool->p_lock);
	struct task *t;
	t->routine=routine;
	t->arg=arg;
	Queue_enqueue(pool->task,t);
	pthread_cond_signal(&pool->p_ready);
	pthread_mutex_unlock(&pool->p_lock);

}
