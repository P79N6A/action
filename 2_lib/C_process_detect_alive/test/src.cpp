#include<iostream>
#include<string.h>
#include<stdio.h>
#define LOG_DEBUG(format,args...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args)
#define LOG_ERROR(format,args...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args)
#include"detect_alive_udp.h"
#include"pp_socket.h"
using namespace std;
		
int main(int argc,char*argv[])
{
	da::DetectAliveUDP du;
	pp::UDPSocket us;
	char a[10][40];
	int n=argc-1;
	for( int i=0;i<n;i++)
	{
		strcpy(a[i],argv[i+1]);
	}
	if(int r=du.init(100))
	{
		cout<<"init error:"<<r<<endl;
		return -1;
	}
	if(int r=du.add_url(n,(char*)a,sizeof(a[0])))
	{
		cout<<"init error:"<<r<<endl;
		return -1;
	}
	if(!us.init(true,false))
	{
		cout<<"socket init error"<<endl;
		return -1;
	}
	while(true)
	{
		static int last=time(0);
		int now=time(0);
		if(now-last>=1)
		{
			char buf[2048];
			last=now;
			da::NetAddr*na=du.get_loop_addr(now);
//			da::NetAddr*na=du.get_mode_addr(100,now);
			if(!na)
			{
				cout<<"get next addr error"<<endl;
			}
			else
			{
				snprintf(buf,sizeof(buf),"%d",na->index);
				int len=strlen(buf);
				if(!us.sendto(buf,len,na->ip,na->port))
				{
					cout<<"send msg error"<<endl;
				}
				else
				{
					cout<<"send msg to:"<<na->ip_str<<endl;
				}
			}
		}
		
		{
			char buf[2048];
			int len;
			unsigned int ip;
			unsigned short port;
			if(!us.recvfrom(buf,sizeof(buf),&len,&ip,&port))
			{
				cout<<"recv from error"<<endl;
			}
			if(len==0)
				continue;
			{
				cout<<"recv feed_back from "<<htons(port)<<":"<<buf<<endl; 	
				int index=atoi(buf);
				//if(int r=du.feed_back_addr(index,now))
				if(int r=du.feed_back_addr(ip,port,now))
				{
					cout<<"feed back error:"<<r<<endl;
				}
			}
		}
	}

	return 0;
}
