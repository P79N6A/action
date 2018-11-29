#include<iostream>
#include"pp_udp.h"
using namespace std;
int main(int argc,char*argv[])
{
	cout<<"I'm client...."<<endl;
	pp::UDP cli;
	
	if(!cli.init(AF_INET6,false,false))
	{
		cout<<"init error:"<<LAST_ERR<<endl;
		return -1;
	}
	if(argc<=1)
	{
		cout<<"wrong format"<<endl;
		return -1;
	}
	int way=atoi(argv[1]);
	pp::UA ua;
	if(!ua.set_url("udp://[fe80::1605:aff:fe33:5c0d%eth1]:20000"))
	{
		cout<<"set url error:"<<LAST_ERR<<endl;
		return -1;
	}
	cout<<ua.get_url()<<endl;
	cout<<ua.get_ip()<<endl;
	cout<<ua.get_port()<<endl;

	string data="hope back";
	if(way==0)
	{
		if(!cli.set_peer(ua))
		{
			cout<<"set peer error:"<<LAST_ERR<<endl;
			return -1;
		}

		if(!cli.sendto(data.c_str(),data.size()))
		{
			cout<<"send data error"<<endl;
			return -1;
		}
	}
	else
	{
		if(!cli.sendto(data.c_str(),data.size(),&ua))
		{
			cout<<"send data error"<<endl;
			return -1;
		}
	}
	pp::UA back;
	char buf[1024];
	int len;
	if(!cli.recvfrom(buf,sizeof(buf),&len,&back))
	{
		cout<<"recvfrom error"<<endl;
	}
	buf[len]='\0';
	cout<<"data echofrom:"<<back.get_url()<<'\t'<<buf<<endl;

	
	return 0;
}

