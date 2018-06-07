#include"threadpool.h"
#include"packet.h"
#include<stdio.h>
int main()
{
	int i;
	thread_pool_t pool;
	pool=thread_pool_create(10);
	//for(i=0;i<20;i++)
	//	thread_pool_add_task(pool,test,(void*)i);
	thread_pool_add_task(pool,receive,"tcp");
	thread_pool_add_task(pool,receive,"udp");	
	thread_pool_add_task(pool,info_print,NULL);
	
	thread_pool_destroy(pool);
	return 0;
}
