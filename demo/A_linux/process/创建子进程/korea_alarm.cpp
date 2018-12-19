#include<iostream>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdio.h>
using namespace std;
int main(int argc,char*argv[])
{
	if(daemon(1,1)==-1)
	{
		cout<<"create daemon error"<<endl;
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sfd==-1)
	{
		cout<<"create socket error"<<endl;
		return-1;
	}
	struct sockaddr_in bind_addr;
	bind_addr.sin_family=AF_INET;
	bind_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind_addr.sin_port=htons(31415);
	if(bind(sfd,(struct sockaddr*)&bind_addr,sizeof(struct sockaddr))==-1)
	{
		cout<<"bind error"<<endl;
		return -1;
	}
	while(true)
	{
		struct sockaddr_in cli_addr;
		char buf[1024];
		int len;
		socklen_t addrlen=sizeof(struct sockaddr);
		if((len=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli_addr,&addrlen))==-1)
		{
			cout<<"recv from error"<<endl;
			return -1;
		}
		buf[len]='\0';
		cout<<"alarm server recv_msg:"<<buf<<endl;
		char sbuf[1024];
		snprintf(sbuf,sizeof(sbuf),"\"%s\"",buf);
		int pid=fork();
		if(pid==-1)
		{
			return -1;
		}
		if(pid==0)
		{
			char*arg[]={"alarm_elf","canshu_123",sbuf,(char*)0};
			execv("/path/alarm_elf",arg);
		}
	}
	return 0;
}
