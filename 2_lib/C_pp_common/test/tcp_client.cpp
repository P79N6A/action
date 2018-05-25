/*
 * tcp_client.cpp

 *
 *  Created on: Dec 2, 2016
 *      Author: focuszhang
 */
#include"log.h"
#include "api_log.h"
#include "api_log_demo.h"
#include "pp_tcp.h"

#include<iostream>
using namespace std;
class Client:public pp::TCPClientSocket
{
public:
	virtual	void	recv(const char *buf,int recv_len)
	{
		cout<<"----------------------------"<<time(0)<<endl;
		cout<<"client recv:"<<buf<<endl;
	}
};
int main()
{
	cout<<"I'm tcp_client"<<endl;
	Client cl;
	if(!cl.init(1))
	{
		cout<<"init error"<<endl;
		return -1;
	}
	if(!cl.connect("tcp://127.0.0.1:1943"))
	{
		cout<<"connect error"<<endl;
		return -1;
	}
	while(true)
	{
		static int last_time=time(0);
		int now=time(0);
		if(now-last_time>=1)
		{
			last_time=now;

			if(!cl.send("brave",6))
			{
				LOG_ERROR("send error");
			}
		}
		cl.update();
	}
	return 0;
}
