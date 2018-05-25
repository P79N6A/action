/*
 * pp_socket.h
 *
 *  Created on: 2015��11��11��
 *      Author: focuszhang
 */

#ifndef APPS_APOLLO_CHAT_PP_COMMON_PP_SOCKET_H_
#define APPS_APOLLO_CHAT_PP_COMMON_PP_SOCKET_H_

#ifdef linux
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <winsock2.h>
//#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
namespace pp
{

class UDPSocket
{
public:
	int		_socket_fd;
	bool	_socket_is_normal;
	struct sockaddr_in _peer_addr;
	struct sockaddr_in _local_addr;
private:
	char	_bind_addr[128];
	unsigned short _bind_port;
	bool _peer_addr_has_set;
public:
	UDPSocket():
		_socket_fd(-1),
		_socket_is_normal(false),
		_bind_port(0),
		_peer_addr_has_set(false)
	{
		memset(&_peer_addr,0,sizeof(_peer_addr));
		memset(&_local_addr,0,sizeof(_local_addr));
	}
	~UDPSocket()
	{
#if linux
		close(_socket_fd);
#elif defined(WIN32) || defined(WIN64)
		closesocket(_socket_fd);
#endif
	}
	bool init(bool nonblock=true,bool check_socket_buf=true)
	{
#ifdef linux
		_socket_fd=socket(AF_INET,SOCK_DGRAM,0);
		if(_socket_fd==-1)
		{
			LOG_ERROR("socket create error[%d]%s",errno,strerror(errno));
			return false;
		}
		if(nonblock)
		{
			int flags=fcntl(_socket_fd,F_GETFL,0);
			if(flags==-1)
			{
				LOG_ERROR("fcntl F_GETFL error");
				return false;
			}
			if (fcntl(_socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
			{
				LOG_ERROR("set socket nonblock failed, so the socket can't be used[%d]%s",errno,strerror(errno));
				return false;
			}
		}
		const int LEN=16000000;
		int optval=LEN;
		socklen_t optlen=sizeof(optval);
		if(setsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(const void*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
			return false;
		}
		if(setsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(const void*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
			return false;
		}
		{
			optval=0;
			optlen=sizeof(optval);
			if(getsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(void*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]%s",errno,strerror(errno));
				return false;
			}
			LOG_DEBUG("UDPSOCKET_SNDBUF:%d",optval);
			if(check_socket_buf && optval<LEN)
			{
				LOG_ERROR("socket send buf is too small:%d<%d",optval,LEN);
				return false;
			}

			if(getsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(void*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]%s",errno,strerror(errno));
				return false;
			}
			LOG_DEBUG("UDPSOCKET_RCVBUF:%d",optval);
			if(check_socket_buf && optval<LEN)
			{
				LOG_ERROR("socket recv buf is too small:%d<%d",optval,LEN);
				return false;
			}
		}

		_socket_is_normal=true;
		return true;
#endif

#if defined(WIN32) || defined(WIN64)
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2,2);
		if(WSAStartup(sockVersion, &wsaData) != 0)
		{
			LOG_ERROR("WSAStart up error[%d]",GetLastError());
		    return false;
		}
	    _socket_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	    if(_socket_fd == INVALID_SOCKET)
	    {
			LOG_ERROR("socket create error[%d]",GetLastError());
			return false;
	    }
	    unsigned long mode=1;
	    if(::ioctlsocket(_socket_fd,FIONBIO,&mode)==-1)
	    {
	    	LOG_ERROR("ioctlsocket error[%d]",GetLastError());
	    	return false;
	    }
		const int LEN=16000000;
		int optval=LEN;
		int optlen=sizeof(optval);
		if(::setsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(const char*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]",GetLastError());
			return false;
		}
		if(::setsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(const char*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]",GetLastError());
			return false;
		}
		{
			optval=0;
			optlen=sizeof(optval);
			if(::getsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(char*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]",GetLastError());
				return false;
			}
			LOG_DEBUG("UDPSOCKET_SNDBUF:%d",optval);
			if(::getsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(char*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]",GetLastError());
				return false;
			}
			LOG_DEBUG("UDPSOCKET_RCVBUF:%d",optval);
		}
		_socket_is_normal=true;
		return true;
#endif
	}
	bool split_url(const char*url,char ip[],unsigned short*pport)
	{
		if(!url)
		{
			LOG_ERROR("the url is empty");
			return false;
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
						LOG_ERROR("the url is too long(>128) or the url format is incomplete:%s",url);
						return false;
					}
				}
				ip[j]='\0';
				if(url[i+1]=='\0')
				{
					LOG_ERROR("when set peer addr,the url has no port defined");
					return false;
				}
				port=atoi(&url[i+1]);
				if(port>65535)
				{
					LOG_ERROR("the prot is not in the normal range");
					return false;
				}
				break;
			}
		}
		if(port==0)
		{
			LOG_ERROR("the url has no valid addr");
			return false;
		}
		*pport=static_cast<unsigned short>(port);
		return true;
	}
	bool bind_url(const char *url)
	{
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		if(!split_url(url,_bind_addr,&_bind_port))
		{
			LOG_ERROR("split_url error:%s",url);
			return false;
		}
#ifdef linux
		_local_addr.sin_family=AF_INET;
		_local_addr.sin_addr.s_addr=inet_addr(_bind_addr);
		_local_addr.sin_port=htons(_bind_port);
		if(::bind(_socket_fd,(struct sockaddr*)&_local_addr,sizeof(_local_addr))==-1)
		{
			LOG_ERROR("bind error[%d]%s,ip:%s,port:%u",errno,strerror(errno),_bind_addr,_bind_port);
			return false;
		}
#elif defined(WIN32) || defined(WIN64)
		_local_addr.sin_family=AF_INET;
		_local_addr.sin_addr.S_un.S_addr=inet_addr(_bind_addr);
		_local_addr.sin_port=htons(_bind_port);
		if(::bind(_socket_fd,(struct sockaddr*)&_local_addr,sizeof(_local_addr))==-1)
		{
			LOG_ERROR("bind error[%d],ip:%s,port:%u",GetLastError(),_bind_addr,_bind_port);
			return false;
		}
#endif
		return true;
	}
	bool set_peer_addr(const char*url)
	{
		char ip[128];
		unsigned short port;


		if(!split_url(url,ip,&port))
		{
			LOG_ERROR("split url error:%s",url);
			return false;
		}
		LOG_DEBUG("peer_addr_set_to_ip:%s port:%d",ip,port);
		_peer_addr.sin_family=AF_INET;
#ifdef linux
		_peer_addr.sin_addr.s_addr=inet_addr(ip);
#elif defined(WIN32) || defined(WIN64)
		_peer_addr.sin_addr.S_un.S_addr=inet_addr(ip);
#endif
		_peer_addr.sin_port=htons(port);

		_peer_addr_has_set=true;
		return true;
	}

	bool recvfrom(char buf[],size_t buf_size,int* plen,uint32_t* pip=0,unsigned short* pport=0)
	{
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		struct sockaddr_in cli_addr;
		struct sockaddr_in* paddr;
		if(_peer_addr_has_set)
		{
			paddr=&cli_addr;
			memset(&cli_addr,0,sizeof(cli_addr));
		}
		else
		{
			paddr=&_peer_addr;
		}
		memset(paddr,0,sizeof(struct sockaddr_in));
#ifdef linux
		socklen_t addrlen=sizeof(cli_addr);
		*plen=::recvfrom(_socket_fd,buf,buf_size,0,(struct sockaddr*)paddr,&addrlen);
		if(*plen==-1)
		{
			if(errno==11)
			{
				*plen=0;
				return true;
			}
			else
			{
				LOG_ERROR("recvfrom error[%d]%s",errno,strerror(errno));
				return false;
			}
		}
#elif defined(WIN32) || defined(WIN64)
		int addrlen=sizeof(cli_addr);
		*plen=::recvfrom(_socket_fd,buf,buf_size,0,(struct sockaddr*)paddr,&addrlen);
		if(*plen==-1)
		{
			if(GetLastError()==10035)
			{
				*plen=0;
				return true;
			}
			else
			{
				LOG_ERROR("recvfrom error[%d]",GetLastError());
				return false;
			}
		}
#endif
		{
			if(pip)
			{
#ifdef linux
				*pip=paddr->sin_addr.s_addr;
#elif defined(WIN32) || defined(WIN64)
				*pip=paddr->sin_addr.S_un.S_addr;
#endif
			}
			if(pport)
			{
				*pport=paddr->sin_port;
			}
		}
		_peer_addr_has_set=true;
		return true;
	}

	bool sendto(const char *buf,int len,unsigned int ip=0,unsigned short port=0)
	{
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		int ret;
		if(ip==0)
		{
			if(!_peer_addr_has_set)
			{
				LOG_ERROR("the dest addr has not set");
				return false;
			}
		}
#ifdef linux
		else
		{
			_peer_addr.sin_family=AF_INET;
			_peer_addr.sin_addr.s_addr=ip;
			_peer_addr.sin_port=port;

			_peer_addr_has_set=true;
		}
		if((ret=::sendto(_socket_fd,buf,len,0,(struct sockaddr*)&_peer_addr,sizeof(_peer_addr)))!=len)
		{
			LOG_ERROR("sendto error:%d[%d]%s",ret,errno,strerror(errno));
			return false;
		}
#elif defined(WIN32) || defined(WIN64)
		else
		{
			_peer_addr.sin_family=AF_INET;
			_peer_addr.sin_addr.S_un.S_addr=ip;
			_peer_addr.sin_port=port;

			_peer_addr_has_set=true;
		}
		if((ret=::sendto(_socket_fd,buf,len,0,(struct sockaddr*)&_peer_addr,sizeof(_peer_addr)))!=len)
		{
			LOG_ERROR("sendto error:%d[%d]",ret,GetLastError());
			return false;
		}
#endif
		return true;
	}
};

inline char *uintip_to_string(unsigned int ip)
{
	struct in_addr ia;
	memcpy(&ia,&ip,4);
	return inet_ntoa(ia);
}
inline char* tostring(char* buf,int buf_size,unsigned int ip,unsigned port)
{
	unsigned char*b=(unsigned char*)&ip;
	snprintf(buf,buf_size,"%d.%d.%d.%d:%hu",b[0],b[1],b[2],b[3],ntohs(port));
	return buf;
}


}


#endif /* APPS_APOLLO_CHAT_PP_COMMON_PP_SOCKET_H_ */
