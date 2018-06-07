#include"threadpool.h"
#include<malloc.h>

void *thread_info(void *arg)
{
	thread_pool_t pool=(thread_pool_t)arg;
	int total,used,free,processing,waiting,finished;
	while(1)	
	{	
		pthread_rwlock_rdlock(&pool->rw_lock);
		total=pool->thread_info.total;
		used=pool->thread_info.used;
		free=pool->thread_info.free;
		printf("\n**************************************************************\n");
		printf("Thread:\t %3d total, %10d used, %6d free\n",total,used,free);
		total=pool->task_info.total;
		processing=pool->task_info.processing;
		waiting=pool->task_info.waiting;
		finished=pool->task_info.finished;	
		printf("Task:  \t %3d total, %3d processsing, %3d waiting, %3d finished\n",total,processing,waiting,finished);
		printf("**************************************************************\n");
		pthread_rwlock_unlock(&pool->rw_lock);
		sleep(1);
	}
}
void *thread_function(void *arg)
{
	thread_pool_t pool=(thread_pool_t)arg;
	while(1)
	{
		pthread_mutex_lock(&pool->task_lock);
		while(isempty(pool->task))
			pthread_cond_wait(&pool->task_ready,&pool->task_lock);
		/*get a task*/
		struct task *t=(struct task*)getFront(pool->task);
		Queue_dequeue(pool->task);	
		pthread_mutex_unlock(&pool->task_lock);
		/*chang info*/
		pthread_rwlock_wrlock(&pool->rw_lock);
		pool->task_info.waiting=Queue_length(pool->task);
		pool->thread_info.used++;
		pool->thread_info.free--;
		pool->task_info.processing++;
		pthread_rwlock_unlock(&pool->rw_lock);
		/*run a task*/
		(*(t->routine))(t->arg);
		pthread_rwlock_wrlock(&pool->rw_lock);
		pool->task_info.processing--;
		pool->task_info.finished++;
		pool->thread_info.used--;
		pool->thread_info.free++;
		pthread_rwlock_unlock(&pool->rw_lock);
		free(t);
	}

}

thread_pool_t thread_pool_create(int n)
{
	int i;
	thread_pool_t pool=NULL;
	pool=(thread_pool_t)malloc(sizeof(struct thread_pool));
	pool->thread_info.total=n;
	pool->thread_info.used=0;
	pool->thread_info.free=n;
	pool->task_info.total=0;
	pool->task_info.processing=0;
	pool->task_info.waiting=0;
	pool->task_info.finished=0;
	pool->task=Queue_create();
	pool->threads=(pthread_t *)malloc(sizeof(pthread_t)*n);
	pthread_mutex_init(&pool->task_lock,NULL);
	pthread_cond_init(&pool->task_ready,NULL);
	pthread_rwlock_init(&pool->rw_lock,NULL);
	pthread_create(&pool->info_id,NULL,thread_info,pool);
	for(i=0;i<n;i++)
		pthread_create(pool->threads+i,NULL,thread_function,pool);
	return pool;
}

void thread_pool_add_task(thread_pool_t pool,void *(*routine)(void *arg),void *arg)
{
	pthread_mutex_lock(&pool->task_lock);
	struct task *t=(struct task*)malloc(sizeof(struct task));
	t->routine=routine;
	t->arg=arg;
	Queue_enqueue(pool->task,t);
	pthread_rwlock_wrlock(&pool->rw_lock);
	pool->task_info.total++;
	pthread_rwlock_unlock(&pool->rw_lock);
	pthread_cond_signal(&pool->task_ready);
	pthread_mutex_unlock(&pool->task_lock);

}

void thread_pool_destroy(thread_pool_t pool)
{
	int i;
	for(i=0;i<pool->thread_info.total;i++)
		pthread_join(pool->threads[i],NULL);
	pthread_join(pool->info_id,NULL);
	pthread_mutex_destroy(&pool->task_lock);
	pthread_cond_destroy(&pool->task_ready);
	pthread_rwlock_destroy(&pool->rw_lock);
	Queue_destroy(pool->task);
	free(pool->threads);
	free(pool);
}
