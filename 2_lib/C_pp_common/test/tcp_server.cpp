/*
 * tcp_server.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: focuszhang
 */
#include"log.h"
#include "api_log.h"
#include "api_log_demo.h"
#include"pp_tcp.h"

#include<iostream>
using namespace std;

class Server:public pp::TCPServerSocket
{
public:
	virtual	void	recv(const int action,const unsigned long long conn,const char *buf,int recv_len)
	{
		switch(action)
		{
		case ACTION_NEW_CONNECTION:
			cout<<"ACTION_NEW_CONNECTION"<<endl;
//			close_connection(conn);
			break;
		case ACTION_DATA_COMES:
			if(buf)
			{
				cout<<"server_recv:"<<buf<<endl;
				if(!this->send(conn,buf,recv_len))
				{
					cout<<"server send error"<<endl;
					return;
				}
				cout<<"server_send:"<<buf<<endl;
			}
			break;
		case ACTION_CLOSE_CONNECTION:
			cout<<"ACTION_CLOSE_CONNECTION"<<endl;
			break;
		}
	}
};
int main()
{
	Server se;
	if(!se.init())
	{
		cout<<"init error"<<endl;
		return -1;
	}
	if(!se.bind_and_listen("tcp://0.0.0.0:1943",2))
	{
		cout<<"bind and listen error"<<endl;
		return -1;
	}
	while(true)
	{
		static int last_time=time(0);
		int now=time(0);
		if(now-last_time>=1)
		{
			last_time=now;
			cout<<"----------------------------------------"<<now<<endl;
			cout<<"connection_count:\t"<<se.get_connection_num()<<endl;
		}
		se.update();
	}
	cout<<"tcp_server"<<endl;
	return 0;
}





