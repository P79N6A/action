/*
 * pp_socket.h
 *
 *  Created on 2018-09-13
 *      Author: purperzhang
 */

#ifndef APPS_APOLLO_CHAT_PP_COMMON_PP_UDP_H_ 
#define APPS_APOLLO_CHAT_PP_COMMON_PP_UDP_H_ 

#include <stdio.h>
#include<string>

#ifdef linux
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include<netdb.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <winsock2.h>
//#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#if ! defined(LOG_ERROR) || ! defined(LOG_DEBUG)

	#define SELF_DEFINE_LOG

	#ifdef linux
	#define LOG_ERROR(format,args...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args)
	#define LOG_DEBUG LOG_ERROR
	
	#elif defined(WIN32) || defined(WIN64)
	#define LOG_ERROR(format,...) printf("[%s:%d]"format"\n",__FILE__,__LINE__,##__VA_ARGS__)
	#define LOG_DEBUG LOG_ERROR

	#endif
#endif

namespace pp
{
class UDP;
struct UB
{
	UB()
	:addrlen(0)
	{}
	unsigned short	_family;
	socklen_t	addrlen;
	union{
//		struct	sockaddr_storage	ss;//avoid too many memmory copy
		struct	sockaddr	sa;
		struct	sockaddr_in	sai;
		struct	sockaddr_in6	sai6;
	}addr;
};
struct UA:public UB
{
public:
	friend class UDP;
private:
	bool			_is_valid;

	std::string	_url;
	std::string	_host;
	std::string	_serv;
	std::string _binary_cache;
public:
	UA()
	:_is_valid(false)
	{}
	UA(const std::string url)
	:_is_valid(false)
	{
		set_url(url);
	}
	UA(const pp::UB&b)
	{
		this->operator =(b);
	}
	operator bool() const 
	{
		return _is_valid;
	}
	bool operator=(const std::string url)
	{
		return set_url(url);
	}
	bool operator=(const char*url)
	{
		return set_url(url);
	}

	bool	set_url(const std::string url)
	{
		clear_string();
		_is_valid=false;
		_url=url;
		if(index(url.c_str(),'['))
			_family=AF_INET6;
		else
			_family=AF_INET;
		if(!UA::split_url(_url,&_host,&_serv))
		{
			LOG_ERROR("split url:%s error",_url.c_str());
			return false;
		}
		struct addrinfo hints;
		struct addrinfo *result;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
		hints.ai_socktype =SOCK_DGRAM; /* Datagram socket */
		hints.ai_flags =0;    /* For wildcard IP address */
		hints.ai_protocol = 0;          /* Any protocol */
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;
		int s = getaddrinfo(_host.c_str(), _serv.c_str(), &hints, &result);
		if (s != 0) {
			LOG_ERROR("getaddrinfo: %s\n", gai_strerror(s));
			return false;
		}
		addrlen=result->ai_addrlen;
		memset(&addr,0,sizeof(addr));
		memcpy(&addr,result->ai_addr,result->ai_addrlen);
		freeaddrinfo(result);

		_is_valid=true;
		return true;
	}
	void set_sockaddr(unsigned short fm,struct sockaddr*psa,int len)
	{
		clear_string();
		_family=fm;
		memcpy(&addr,psa,len);
		addrlen=len;
		_is_valid=true;

	}

	pp::UA& operator=(const pp::UB&ub)
	{
		clear_string();
		*(pp::UB*)this=ub;
		_is_valid=true;
		return *this;
	}
	void set_ipv4(unsigned int ip,unsigned short port)
	{
		clear_string();
		_family=AF_INET;
		addr.sai.sin_family=AF_INET;
		addr.sai.sin_addr.s_addr=ip;
		addr.sai.sin_port=port;
		addrlen=sizeof(addr.sai);
		_is_valid=true;
	}
	bool	set_port(unsigned short port)
	{
		if(!_is_valid)
		{
			LOG_ERROR("the udp address is not valid");
			return false;
		}
		else
		{
			clear_string();
			if(_family==AF_INET)
			{
				addr.sai.sin_port=htons(port);
			}
			else if(_family==AF_INET6)
			{
				addr.sai6.sin6_port=htons(port);
			}
			else
			{
				LOG_ERROR("wrong family:%d",_family);
				return false;
			}
		}
		return true;
	}
	void clear_string()
	{
		_url.clear();
		_host.clear();
		_serv.clear();
		_binary_cache.clear();
	}
	bool complete_string()
	{
		if(!_is_valid)
		{
			LOG_ERROR("the socket address is not valid");
			return false;
		}
		_url.clear();
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];
		int r=getnameinfo((struct sockaddr*)&addr,addrlen,host,sizeof(host),serv,sizeof(serv),NI_NUMERICHOST|NI_NUMERICSERV);
		if(!r)
		{
			if(_family==AF_INET)
				_url=_url+"udp://"+host+":"+serv;
			else
				_url=_url+"udp://["+host+"]:"+serv;
		}
		else
		{
			LOG_ERROR("getnameinfo error:%d",r);
			return false;
		}
		_host=host;
		_serv=serv;
		return true;
	}
	bool is_ipv4()
	{
		if(!_is_valid)
			return false;
		return _family==AF_INET;
	}
	bool is_ipv6()
	{
		if(!_is_valid)
			return false;
		return _family==AF_INET6;
	}
	const std::string& get_url()
	{
		if(_url.empty())
			complete_string();
		return _url;

	}
	const std::string& get_ip()
	{
		if(_host.empty())
			complete_string();
		return _host;
	}
	const std::string& get_port()
	{
		if(_serv.empty())
			complete_string();
		return _serv; 
	}
	unsigned	get_ipv4_int_ip()
	{
		if(!_is_valid || _family!=AF_INET)
			return 0;
		return addr.sai.sin_addr.s_addr;
	}
	unsigned short get_ipv4_int_port()
	{
		if(!_is_valid || _family!=AF_INET)
			return 0;
		return addr.sai.sin_port;
	}
	unsigned short get_family()
	{
		return _family;
	}
	inline const std::string& get_binary()
	{
		if(_binary_cache.empty())
		{
			if(!_is_valid)
			{
				LOG_ERROR("the udp addr is invalid");
			}
			else
			{
				if(_family==AF_INET)
				{
					_binary_cache.assign((const char*)&addr,sizeof(addr.sai.sin_family)+sizeof(addr.sai.sin_port)+sizeof(addr.sai.sin_addr));
				}
				else
				{
					_binary_cache.assign((const char*)&addr,sizeof(addr.sai6));
				}
			}
		}
		return _binary_cache;
	}

	static bool split_url(const std::string& url,std::string*ip,std::string*pport)
	{
		if(url.empty())
		{
			LOG_ERROR("url is empty");
			return false;
		}
		if(!index(url.c_str(),'['))
		{
			//check format;
			if(url.size()>64)
			{
				LOG_ERROR("ipv4 url too long:%zd",url.size());
				return false;
			}
			const char*a=index(url.c_str(),'/');
			const char*b=rindex(url.c_str(),':');
			a+=2;
			if(!a || !b)
			{
				LOG_ERROR("wrong format:%s",url.c_str());
				return false;
			}
			(*ip).assign(a,b-a);
			(*ip).push_back('\0');
			(*pport)=b+1;
		}
		else
		{
			//check format;
			if(url.size()>256)
			{
				LOG_ERROR("ipv6 url too long:%zd",url.size());
				return false;
			}
			const char*a=index(url.c_str(),'[');
			const char*b=index(url.c_str(),']');
			if(!a || !b)
			{
				LOG_ERROR("ipv6 url wrong format");
				return false;
			}
			a+=1;
			(*ip).assign(a,b-a);
			(*ip).push_back('\0');
			(*pport)=b+2;
		}

		return true;
	}
//	int compare(const UA&u2) const
//	{
//#define TMPCOMPARE(a,b) if(a!=b)return (int)a-(int)b
//
//		TMPCOMPARE(addrlen,u2.addrlen);
//		TMPCOMPARE(_family,u2._family);
//		if(_family==AF_INET)
//		{
//			TMPCOMPARE(sai.sin_family,u2.sai.sin_family);
//			TMPCOMPARE(sai.sin_addr.s_addr,u2.sai.sin_addr.s_addr);
//			TMPCOMPARE(sai.sin_port,u2.sai.sin_port);
//		}
//		else
//		{
//			TMPCOMPARE(sai6.sin6_family,u2.sai6.sin6_family);
//			unsigned sz=sizeof(sai6.sin6_addr)/sizeof(int);//sizeof(sai6.sin6_addr)=16
//			for(unsigned i=0;i<sz;i++)
//			{
//				TMPCOMPARE(*((int*)sai6.sin6_addr+i),*((int*)u2.sai6.sin6_addr+i));
//			}
//			TMPCOMPARE(sai6.sin6_flowinfo,u2.sai6.sin6_flowinfo);
//			TMPCOMPARE(sai6.sin6_port,u2.sai6.sin6_port);
//			TMPCOMPARE(sai6.sin6_scope_id,u2.sai6.sin6_scope_id);
//		}
//#undef TMPCOMPARE
//		return 0;
//	}



	//the following function may have bugs in map
//	bool operator<( UA&u2)
//	{
////		return compare(u2)<0;
//		return this->get_binary()<u2.get_binary();
//	}
//	bool operator==( UA&u2)
//	{
////		return compare(u2)==0;
//		return this->get_binary()==u2.get_binary();
//	}
//	bool operator>( UA&u2)
//	{
////		return compare(u2)>0;
//		return this->get_binary()>u2.get_binary();
//	}

};

class UDP
{
public:
	int	_socket_fd;
	int	_family;
	bool	_socket_is_normal;
	bool	_peer_addr_has_set;
	UA	_local_addr;
	UA	_peer_addr;
public:
	UDP():
		_socket_fd(0),
		_socket_is_normal(false),
		_peer_addr_has_set(false)
	{
	}
	void release()
	{
#if linux
		close(_socket_fd);
#elif defined(WIN32) || defined(WIN64)
		closesocket(_socket_fd);
#endif
		_socket_fd=0;
	}
	bool init(unsigned short family,bool nonblock=true,bool check_socket_buf=true)
	{
#ifdef linux
		_family=family;
		_socket_fd=socket(_family,SOCK_DGRAM,0);
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
		const int RLEN=16000000;
		const int SLEN=500000;//according to 1000 txqueuelen
		int optval=SLEN;
		socklen_t optlen=sizeof(optval);
		if(setsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(const void*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
			return false;
		}
		optval=RLEN;
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
//			if(check_socket_buf && optval!=SLEN)
//			{
//				LOG_ERROR("socket send buf is not expected:%d!=%d",optval,SLEN);
//				return false;
//			}

			if(getsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(void*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]%s",errno,strerror(errno));
				return false;
			}
			LOG_DEBUG("UDPSOCKET_RCVBUF:%d",optval);
			if(check_socket_buf && optval<RLEN)
			{
				LOG_ERROR("socket recv buf is too small:%d<%d",optval,RLEN);
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
		const int RLEN=16000000;
		const int SLEN=500000;
		int optval=SLEN;
		int optlen=sizeof(optval);
		if(::setsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(const char*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]",GetLastError());
			return false;
		}
		optval=RLEN;
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
	bool bind(const UA&ua)
	{
		if(&_local_addr!=&ua)
			_local_addr=ua;
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		if(ua._family!=_family)
		{
			LOG_ERROR("family mismatch");
			return false;
	}
		_local_addr=ua;
		if(::bind(_socket_fd,&_local_addr.addr.sa,_local_addr.addrlen)==-1)
		{
			LOG_ERROR("bind error[%d]%s,%s",errno,strerror(errno),_local_addr.get_url().c_str());
			return false;
		}
		return true;
	}
	bool set_peer(const UA&ua)
	{
		if(ua._family!=_family)
		{
			LOG_ERROR("family mismatch");
			return false;
		}
		_peer_addr=ua;
		_peer_addr_has_set=true;
		return true;
	}

	bool recvfrom(char buf[],size_t buf_size,int* plen,UA*pua=0)
	{
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		if(pua)pua->addrlen=sizeof(pua->addr);
		*plen=::recvfrom(_socket_fd,buf,buf_size,0,pua?&pua->addr.sa:0,pua?&pua->addrlen:0);
		if(*plen==-1)
		{
			if(errno==EAGAIN)
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
		if(pua)
		{
			pua->_is_valid=true;
			pua->_family=_family;
			pua->clear_string();
		}
		return true;
	}

	bool sendto(const char *buf,int len,UA*pua=0)
	{
		if(!_socket_is_normal)
		{
			LOG_ERROR("the socket hasn't init yet");
			return false;
		}
		int ret;
		if(pua==0)
		{
			if(!_peer_addr_has_set)
			{
				LOG_ERROR("the dest addr has not set");
				return false;
			}
			else
				pua=&_peer_addr;
		}
		else
		{
			if(pua->_family!=_family)
			{
				LOG_ERROR("dst sockaddr family is not match with the send socket");
				return false;
			}
		}
		if((ret=::sendto(_socket_fd,buf,len,0,&pua->addr.sa,pua->addrlen))!=len)
		{
			LOG_ERROR("sendto error:%d[%d]%s (%s)",ret,errno,strerror(errno),pua->get_url().c_str());
			return false;
		}
		return true;
	}
};

}


#ifdef SELF_DEFINE_LOG
	#undef LOG_ERROR
	#undef LOG_DEBUG
	#undef SELF_DEFINE_LOG
#endif


#endif /* APPS_APOLLO_CHAT_PP_COMMON_PP_UDP_H_ */
