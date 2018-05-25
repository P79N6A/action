#include<iostream>
#include<string.h>
#include<stdio.h>
#define LOG_DEBUG(format,args...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args)
#define LOG_ERROR(format,args...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args)
#include"pp_socket.h"

using namespace std;
int main(int argc,char*argv[])
{
	if(argc<=1)
	{
		cout<<"wrong argument num:"<<argc<<endl;
		return -1;
	}
	pp::UDPSocket us;
	if(!us.init(false,false))
	{
		cout<<"udp init error"<<endl;
		return -1;
	}
	if(!us.bind_url(argv[1]))
	{
		cout<<"bind url error:"<<argv[1]<<endl;
		return -1;
	}
	while(true)
	{
		char buf[2048];
		int len;
		unsigned int ip;
		unsigned short port;
		if(!us.recvfrom(buf,sizeof(buf),&len,&ip,&port))
		{
			cout<<"recv_from error"<<endl;
			return -1;
		}
		cout<<"recv msg:"<<buf<<endl;
		if(!us.sendto(buf,len,ip,port))
		{
			cout<<"send to error"<<endl;
			return -1;
		}	
	}
		
	return 0;
}

