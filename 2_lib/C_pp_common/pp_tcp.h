//20161201 by purperzhang


#ifndef APPS_APOLLO_CHAT_PP_COMMON_PP_TCP_H_
#define APPS_APOLLO_CHAT_PP_COMMON_PP_TCP_H_

#ifdef linux
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <winsock2.h>
//#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include<string>
namespace pp
{

class NetAddr
{
private:
	std::string  url;
public:
	unsigned int& ip;
	unsigned short& port;
	struct sockaddr_in addr;
	NetAddr()
	:ip(*(unsigned int*)&addr.sin_addr.s_addr)
	,port(*(unsigned short*)&addr.sin_port)
	{
		addr.sin_family=AF_INET;
		addr.sin_addr.s_addr=0;
		addr.sin_port=0;
	}
	NetAddr(const char*u)
	:ip(*(unsigned int*)&addr.sin_addr.s_addr)
	,port(*(unsigned short*)&addr.sin_port)
	{
		url=u;
		addr.sin_family=AF_INET;
		char b[64];
		b[0]='\0';
		unsigned short p=0;
		split_url(u,b,&p);
		addr.sin_addr.s_addr=inet_addr(b);
		addr.sin_port=htons(p);
	}
	NetAddr(unsigned int aip,unsigned short aport)
	:ip(*(unsigned int*)&addr.sin_addr.s_addr)
	,port(*(unsigned short*)&addr.sin_port)
	{
		addr.sin_family=AF_INET;
		ip=aip;
		port=aport;
	}

	NetAddr&operator=(const NetAddr&na)
	{
		url=na.url;
		addr=na.addr;
		return *this;
	}
	NetAddr&operator=(const char*u)
	{
		NetAddr na(u);
		*this=na;
		return *this;
	}
	NetAddr&operator=(const std::string& u)
	{
		NetAddr na(u.c_str());
		*this=na;
		return *this;
	}
	std::string to_udp_url()
	{
		if(!url.empty())
			return url;

		char buf[64];
		unsigned char*b=(unsigned char*)&ip;
		snprintf(buf,sizeof(buf),"udp://%d.%d.%d.%d:%hu",b[0],b[1],b[2],b[3],ntohs(port));
		url=buf;
		return url;
	}
	std::string to_tcp_url()
	{
		if(!url.empty())
			return url;

		char buf[64];
		unsigned char*b=(unsigned char*)&ip;
		snprintf(buf,sizeof(buf),"tcp://%d.%d.%d.%d:%hu",b[0],b[1],b[2],b[3],ntohs(port));
		url=buf;
		return url;
	}
	int split_url(const char*url,char ip[],unsigned short*pport)
	{
		if(!url)
		{
			return -112;
		}

		int port=0;
		int j=0;
		for(int i=0;i<128;i++)
		{
			if(url[i]=='\0')
			{
				break;
			}
			if(!isdigit(url[i]))
			{
				continue;
			}

			{
				while(url[i]!=':')
				{
					ip[j++]=url[i++];
					if(i>128||url[i]=='\0')
					{
						return -130;
					}
				}
				ip[j]='\0';
				if(url[i+1]=='\0')
				{
					return -131;
				}
				port=atoi(&url[i+1]);
				if(port<=1000||port>65535)
				{
					return -132;
				}
				break;
			}
		}
		if(port==0)
		{
			return -133;
		}
		*pport=static_cast<unsigned short>(port);
		return 0;
	}
};
//


class TCPSocket
{
public:
	int			_socket_fd;
	NetAddr		net_addr;
	TCPSocket()
	:_socket_fd(0)
	{}
	int	create_socket()
	{
		_socket_fd=socket(AF_INET,SOCK_STREAM,0);
		if(_socket_fd==-1)
		{
			return errno;
		}
		int optval=1;
		if(-1==setsockopt(_socket_fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(optval)))
		{
			return errno;
		}

	//the following  code is not as that important
	//	optval=1;
	//	if(-1==setsockopt(_socket_fd,IPPROTO_TCP,TCP_NODELAY,(const void*)&optval,sizeof(optval)))
	//	{
	//		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
	//		return false;
	//	}
		return 0;
	}
	void close_socket()
	{
		close(_socket_fd);
		_socket_fd=0;
	}
};

}//namespace



#endif /* APPS_APOLLO_CHAT_PP_COMMON_PP_SOCKET_H_ */
