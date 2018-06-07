#include"threadpool.h"
#include<malloc.h>

void *thread_info(void *arg)
{
	thread_pool_t pool=(thread_pool_t)arg;
	while(1)	
	{	
		pthread_rwlock_rdlock(&pool->lock);
		int available=pool->thread_count-pool->live_num;
		printf("\ttotal\tused\tfree\n",pool->thread_count,pool->live_num,available,pool->task_num);
		printf("Thread:\t %d\t %d\t %d\n",pool->thread_count,pool->live_num,available);
		printf("Task:  \t %d\n",pool->task_num);
		pthread_rwlock_unlock(&pool->lock);
		sleep(1);
	}
}
void *thread_function(void *arg)
{
	thread_pool_t pool=(thread_pool_t)arg;
	while(1)
	{
		pthread_mutex_lock(&pool->p_lock);
		while(isempty(pool->task))
			pthread_cond_wait(&pool->p_ready,&pool->p_lock);
		struct task *t=(struct task*)getFront(pool->task);
		Queue_dequeue(pool->task);
		pool->task_num=Queue_length(pool->task);
		pthread_mutex_unlock(&pool->p_lock);
		pthread_rwlock_wrlock(&pool->lock);
		pool->live_num++;
		pthread_rwlock_unlock(&pool->lock);
		(*(t->routine))(t->arg);
		pthread_rwlock_wrlock(&pool->lock);
		pool->live_num--;
		pthread_rwlock_unlock(&pool->lock);
		free(t);
	}

}

thread_pool_t thread_pool_create(int n)
{
	int i;
	thread_pool_t pool=NULL;
	pool=(thread_pool_t)malloc(sizeof(struct thread_pool));
	pool->thread_count=n;
	pool->live_num=0;
	pool->task_num=0;
	pool->task=Queue_create();
	pool->threads=(pthread_t *)malloc(sizeof(pthread_t)*n);
	pthread_mutex_init(&pool->p_lock,NULL);
	pthread_cond_init(&pool->p_ready,NULL);
	pthread_rwlock_init(&pool->lock,NULL);
	pthread_create(&pool->info_id,NULL,thread_info,pool);
	for(i=0;i<n;i++)
		pthread_create(pool->threads+i,NULL,thread_function,pool);
	return pool;
}

void thread_pool_add_task(thread_pool_t pool,void *(*routine)(void *arg),void *arg)
{
	pthread_mutex_lock(&pool->p_lock);
	struct task *t=(struct task*)malloc(sizeof(struct task));
	t->routine=routine;
	t->arg=arg;
	Queue_enqueue(pool->task,t);
	pthread_cond_signal(&pool->p_ready);
	pthread_mutex_unlock(&pool->p_lock);

}

void thread_pool_destroy(thread_pool_t pool)
{
	int i;
	for(i=0;i<pool->thread_count;i++)
		pthread_join(pool->threads[i],NULL);
	pthread_join(pool->info_id,NULL);
	pthread_mutex_destroy(&pool->p_lock);
	pthread_cond_destroy(&pool->p_ready);
	pthread_rwlock_destroy(&pool->lock);
	Queue_destroy(pool->task);
	free(pool->threads);
	free(pool);
}
