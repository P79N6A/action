#include<errno.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<setjmp.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
using namespace std;
	
int create_socket(unsigned short port)
{
	struct sockaddr_in lis;
	memset(&lis,0,sizeof(lis));
	lis.sin_family=AF_INET;
	lis.sin_addr.s_addr=INADDR_ANY;
	lis.sin_port=htons(port);
	int sfd1=socket(AF_INET,SOCK_DGRAM,0);
	if(sfd1==-1)
	{
		printf("socket error[%d]%s\n",errno,strerror(errno));
		return -1;
	}
	if(bind(sfd1,(struct sockaddr*)&lis,sizeof(struct sockaddr))==-1)
	{
		printf("bind error[%d]%s\n",errno,strerror(errno));
		return -1;
	}
	return sfd1;
}

	
int main()
{
	fd_set rfd;
	FD_ZERO(&rfd);
	int sfd1=create_socket(10000);
	int sfd2=create_socket(10001);
	if(sfd1==-1 || sfd2==-1)
	{
		return -1;
	}
	while(true)
	{
		FD_SET(sfd1,&rfd);
		FD_SET(sfd2,&rfd);
		struct timeval tv;
		tv.tv_sec=0;
		tv.tv_usec=10000;
		int rs=select((sfd1>sfd2?sfd1:sfd2)+1,&rfd,0,0,&tv);
		if(rs==-1)
		{
			printf("select error[%d]\n",errno);
			return -1;
		}
		if(FD_ISSET(sfd1,&rfd))
		{
			char buf[1024];
			int  len=0;
			len=recvfrom(sfd1,buf,sizeof(buf),0,0,0);
			buf[len]='\0';
			printf("sfd1 recv[%d]%s\n",len,buf);		
		}
		if(FD_ISSET(sfd2,&rfd))
		{
			char buf[1024];
			int  len=0;
			len=recvfrom(sfd2,buf,sizeof(buf),0,0,0);
			buf[len]='\0';
			printf("sfd2 recv[%d]%s\n",len,buf);		
		}
	}
	return 0;
}


