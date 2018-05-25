#include<iostream>
#include<stdio.h>
#define LOG_ERROR
#define LOG_DEBUG
#include"pp_socket.h"
using namespace std;
int main()
{
	pp::UDPSocket us;
	if(!us.init(true,false))
	{
		cout<<"socket init error"<<endl;
		return -1;
	}
	if(!us.set_peer_addr("udp://127.0.0.1:12345"))
	{
		cout<<"set peer addr error"<<endl;
		return -1;
	}
	int n=0;
	char buf[1024]={'a'};
	while(true)
	{
		int now=time(0);
		static int last=0;
		if(abs(now-last)>=1)
		{
			n=10000;
			last=now;
		}
		while(n-->0)
			if(!us.sendto(buf,1000))
			{
				cout<<"sendto error"<<endl;
			}
	}

	
	return 0;
}
