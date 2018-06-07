#ifndef _packet_h
#define _packet_h

#include<pcap.h>
#include<time.h>

int packet_num;
double elapse_time,last_time;
struct timeval first_packet_time,last_packet_time,start,now;

pcap_t *gethandle();
void analysis(u_char *usr,const struct pcap_pkthdr *pkthdr,const u_char *packet);
void *receive(void *arg);
void *info_print(void *arg);
#endif
