#include"packet.h"
#include<malloc.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<linux/ip.h>
#include<linux/tcp.h>
#include<netinet/if_ether.h>
#include<string.h>
double diff(struct timeval tv1,struct timeval tv2);
char *format="%Y-%m-%d %H:%M:%S";
void err_sys(char *info,char *e);
char *format_time(char *format,time_t rtime);
pcap_t *gethandle()
{
	char ebuf[PCAP_ERRBUF_SIZE];
	char *device;
	pcap_t *p,*handle=NULL;
	device=pcap_lookupdev(ebuf);
	if(!device)
	{
		err_sys("Can't find available device:",ebuf);
		return NULL;
	}
	p=pcap_open_live(device,65535,1,0,ebuf);
	if(!p)
	{
		err_sys("Can't get handle:",ebuf);
		return NULL;
	}
	handle=(pcap_t *)malloc(sizeof(pcap_t *));
	if(handle==NULL)
	{
		err_sys("handle memory allocation failed!\n",NULL);
		return NULL;
	}
	handle=p;
	return handle;
}
void analysis(u_char *usr,const struct pcap_pkthdr *pkthdr,const u_char *packet)
{	
	/*analysis ip header*/
	struct iphdr *ip_h;
	struct in_addr saddr,daddr;
	ip_h=(struct iphdr*)(packet+sizeof(struct ether_header));
	saddr.s_addr=ip_h->saddr;
	printf("Src:%s,",inet_ntoa(saddr));
	daddr.s_addr=ip_h->daddr;
	printf("Dst:%s,",inet_ntoa(daddr));
	printf("Protocol:%d,",ip_h->protocol);
	/*analysis tcp header*/
	struct tcphdr *tcp_h;
	tcp_h=(struct tcphdr*)(packet+sizeof(struct ether_header)+sizeof(struct iphdr));
	printf("Src Port:%d,",ntohs(tcp_h->source));
	printf("Dst Port:%d,",ntohs(tcp_h->dest));
	/*analysis packet header*/
	printf("Len:%d,",pkthdr->len);
	printf("Rev Time:%s\n",format_time(format,pkthdr->ts.tv_sec));
	packet_num++;
	if(packet_num==1)
	{
		first_packet_time=pkthdr->ts;
		last_packet_time=pkthdr->ts;
	}
	else
		last_packet_time=pkthdr->ts;
		
}
void *receive(void *arg)
{
	int err=0;
	pcap_t *handle=gethandle();
	gettimeofday(&start,NULL);
	char *buf=(char *)arg;
	struct bpf_program fp;
	err=pcap_compile(handle,&fp,buf,1,0);
	err=pcap_setfilter(handle,&fp);
	err=pcap_loop(handle,-1,analysis,NULL);
	if(err!=1)
		err_sys("get packet failed!",strerror(err));
	return 0;
}
void *info_print(void *arg)
{	
	while(1)
	{
		if(first_packet_time.tv_sec>0)
		{
			gettimeofday(&now,NULL);
			last_time=diff(now,start);	
			elapse_time=diff(last_packet_time,first_packet_time);
			printf("\ninformation:\n");
			printf("Packet Num:%ld,",packet_num);
			printf("Last Time:%lfsec,",last_time);
			double ave=(double)(packet_num/last_time);
			printf("avg.packets:%lf/sec\n",ave);
			printf("First packet:%s,",format_time(format,first_packet_time.tv_sec));
			printf("Last packet:%s,",format_time(format,last_packet_time.tv_sec));
			printf("Elapsed:%lfsec\n\n",elapse_time);
		}
		sleep(1);
	}
}
char *format_time(char *format,time_t rtime)
{
	struct tm *ptime=localtime(&rtime);
	char ftime[20];
	strftime(ftime,sizeof(ftime),format,ptime);
	char *time=NULL;
	time=(char *)malloc(sizeof(char));
	if(time==NULL)
	{
		printf("memory allocation failed!\n");
		return NULL;
	}
	strcpy(time,ftime);
	return time;	
}
void err_sys(char *info,char *e)
{
	if(e==NULL)
		printf("%s\n",info);
	else
		printf("%s %s\n",info,e);
}

double diff(struct timeval tv1,struct timeval tv2)
{
	long sec=tv1.tv_sec-tv2.tv_sec;
	double dec=(double)(tv1.tv_usec-tv2.tv_usec)/1000000.0;
	double time=sec+dec;
	return time;
}

