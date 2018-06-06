#include"threadpool.h"
#include<stdio.h>
void *test(void *arg)
{
	int *id=(int *)arg;
	printf("id:%ld task:%d\n",pthread_self(),id);
}
int main()
{
	int i;
	thread_pool_t pool;
	pool=thread_pool_create(10);
	for(i=0;i<5;i++)
		thread_pool_add_task(pool,test,(void*)i);
	return 0;
}
