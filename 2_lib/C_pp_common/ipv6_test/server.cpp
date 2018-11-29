#include<iostream>
#include"pp_udp.h"
using namespace std;
int main()
{

	cout<<"I'm server..."<<endl;
	pp::UDP sv;
	if(!sv.init(AF_INET6,false,false))
	{
		cout<<"init error"<<endl;
	}
	pp::UA ba("udp://[fe80::1605:aff:fe33:5c0d%eth1]:20000");
	if(!ba)
	{
		cout<<"set url error:"<<LAST_ERR<<endl;
		return -1;
	}
	if(!sv.bind(ba))
	{
		cout<<"bind url error:"<<LAST_ERR<<endl;
		return -1;
	}
	pp::UA cl;
	char buf[1024];
	int len;
	while(true)
	{
		if(!sv.recvfrom(buf,sizeof(buf),&len,&cl))
		{
			cout<<"recvfrom error"<<endl;
			continue;
		}
		if(len==0)
			continue;
		cout<<"server recvfrom:"<<cl.get_url()<<endl;
		if(!sv.sendto(buf,len,&cl))
		{
			cout<<"send to error"<<endl;
		}
	}
	
	
	return 0;
}
