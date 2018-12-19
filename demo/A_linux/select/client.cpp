#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
int main(int argc,char*argv[])
{
	struct sockaddr_in dst;
	memset(&dst,0,sizeof(dst));
	dst.sin_family=AF_INET;
	dst.sin_addr.s_addr=inet_addr("127.0.0.1");
	dst.sin_port=htons(atoi(argv[1]));
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd==-1)
	{
		printf("socket error\n");
		return-1;
	}
	while(true)
	{
		static int last_time=0;
		int now=time(0);
		if(now-last_time>=1)
		{
			last_time=now;

			char buf[1024];
			int len=snprintf(buf,sizeof(buf),"pid[%d]",getpid());
			if(sendto(fd,buf,len,0,(struct sockaddr*)&dst,sizeof(struct sockaddr))==-1)
			{
				printf("send to error[%d]\n",errno);
				return -1;
			}
		}
	}
		
	return 0;
}
