#include"threadpool.h"
#include<stdio.h>
#include<time.h>
void *test(void *arg)
{
	int *id=(int *)arg;
	int i;
	for(i=0;i<3;i++)
	{
		printf("id:%ld task:%d\n",pthread_self(),id);
		sleep((int)id);
	}
}
int main()
{
	int i;
	thread_pool_t pool;
	pool=thread_pool_create(10);
	for(i=0;i<20;i++)
		thread_pool_add_task(pool,test,(void*)i);
	thread_pool_destroy(pool);
	return 0;
}
