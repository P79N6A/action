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
#include"purperzhang.h"
#include"pp_type.h"


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

struct TB
{
	TB()
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
struct TA:public TB
{
public:
	friend class TCPC;
	friend class TCPS;
private:
	bool			_is_valid;

	std::string	_url;
	std::string	_host;
	std::string	_serv;
	std::string _binary_cache;
public:
	TA()
	:_is_valid(false)
	{}
	TA(const std::string url)
	:_is_valid(false)
	{
		set_url(url);
	}
	TA(const pp::TB&b)
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
		if(!TA::split_url(_url,&_host,&_serv))
		{
			LOG_ERROR("split url:%s error",_url.c_str());
			return false;
		}
		struct addrinfo hints;
		struct addrinfo *result;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
		hints.ai_socktype =SOCK_STREAM; /* Datagram socket */
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

	pp::TA& operator=(const pp::TB&tb)
	{
		clear_string();
		*(pp::TB*)this=tb;
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
				_url=_url+"tcp://"+host+":"+serv;
			else
				_url=_url+"tcp://["+host+"]:"+serv;
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
	bool operator<( TA&t2)
	{
//		return compare(u2)<0;
		return this->get_binary()<t2.get_binary();
	}
	bool operator==( TA&t2)
	{
//		return compare(u2)==0;
		return this->get_binary()==t2.get_binary();
	}
	bool operator>( TA&t2)
	{
//		return compare(u2)>0;
		return this->get_binary()>t2.get_binary();
	}
};

class TCP
{
public:
	int		_socket_fd;
	TA		tcp_addr;
	TCP()
	:_socket_fd(0)
	{}
	bool init(unsigned short family)
	{
		_socket_fd=socket(family,SOCK_STREAM,0);
		if(_socket_fd==-1)
		{
			LOG_ERROR("socket create error[%d]%s",errno,strerror(errno));
			return false;
		}
		int optval=1;
		if(-1==setsockopt(_socket_fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(optval)))
		{
			LOG_ERROR("socket opt set error[%d]%s",errno,strerror(errno));
			return false;
		}

	//the following  code is not as that important
	//	optval=1;
	//	if(-1==setsockopt(_socket_fd,IPPROTO_TCP,TCP_NODELAY,(const void*)&optval,sizeof(optval)))
	//	{
	//		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
	//		return false;
	//	}
		return true;
	}
	bool set_nonblock()
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
			return true;
	}
	bool set_socket_buf(int LEN=0)
	{
		if(LEN<=0)
			return true;

		int optval=LEN;
		socklen_t optlen=sizeof(optval);
//		if(setsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(const void*)&optval,optlen)==-1)
//		{
//			LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
//			return false;
//		}
		if(setsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(const void*)&optval,optlen)==-1)
		{
			LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
			return false;
		}
		{
			optval=0;
			optlen=sizeof(optval);
//			if(getsockopt(_socket_fd,SOL_SOCKET,SO_SNDBUF,(void*)&optval,&optlen)==-1)
//			{
//				LOG_ERROR("getsockopt error[%d]%s",errno,strerror(errno));
//				return false;
//			}
//			LOG_DEBUG("TCPSOCKET_SNDBUF:%d",optval);
//			if(  optval<LEN)
//			{
//				LOG_ERROR("socket send buf is too small:%d<%d",optval,LEN);
//				return false;
//			}

			if(getsockopt(_socket_fd,SOL_SOCKET,SO_RCVBUF,(void*)&optval,&optlen)==-1)
			{
				LOG_ERROR("getsockopt error[%d]%s",errno,strerror(errno));
				return false;
			}
			LOG_DEBUG("TCPSOCKET_RCVBUF:%d",optval);
			if(  optval<LEN)
			{
				LOG_ERROR("socket recv buf is too small:%d<%d",optval,LEN);
				return false;
			}
		}
		return true;
	}
	void release()
	{
		if(_socket_fd!=0 && _socket_fd!=-1)
			close(_socket_fd);
		_socket_fd=-1;
	}
};

struct Conn:public pp::Brick
{
	TCP		tsock;
	uint64	key;	//when key=0 conn is in unused, when key!=0 conn is in active
	const unsigned	index;
	int		last_active_time;

	std::string udata;
	union
	{
		void*	uptr;
		uint64	u64;
		uint32	u32;
		int		fd;

	};
	bool	is_timeout;
	Conn(unsigned i)
	:index(i)
	{}
	void clear()
	{
		tsock.release();
		key=0;
		last_active_time=0;
		udata.clear();
		uptr=0;
		//index can't be set, for it is used mainly for index;
		is_timeout=false;
	}
};

class TCPS
{
public:
	enum{
		MAX_EVENTS=1000000
	};
	TCP		bt;
	Conn*	conn_array;
	unsigned	conn_count;
	pp::BrickSet	unused;
	pp::BrickSet	active;
	int efd;
	struct epoll_event events[MAX_EVENTS];

	int	 c_socket_buf;
	bool c_is_nonblock;
	int	c_timeout;
	unsigned	c_max_string;
	unsigned c_key;

private:
	char _buf[10240];
	int _len;
	void (*on_new_conn)(Conn*conn);
	void (*on_close_conn)(Conn*conn);
	void (*on_data_come)(Conn*conn);//should recv data as many as possile in this function
public:
	TCPS()
	:unused(pp::Brick::link_use_offset)
	,active(pp::Brick::link_use_offset)
	{
		c_socket_buf=0;
		c_is_nonblock=false;
		c_timeout=180;
		c_max_string=0;
		c_key=0;
		on_new_conn=0;
		on_close_conn=0;
		on_data_come=0;
	}
	bool	init(unsigned conn_num)
	{
		if(conn_num<=0)
		{
			LOG_ERROR("conn_num can't be zero");
			return false;
		}
		conn_count=conn_num;
		conn_array=(Conn*)new(std::nothrow)char[conn_count*sizeof(Conn)];
		if(!conn_array)
		{
			LOG_ERROR("new connection error");
			return false;
		}
		for(unsigned i=0;i<conn_count;i++)
		{
//			conn_array[i].index=i;
			new(conn_array+i)Conn(i);
			unused.add(conn_array+i);
		}

		//socket
		efd=epoll_create(MAX_EVENTS);
		if(efd==-1)
		{
			LOG_ERROR("epoll_create error");
			return false;
		}
		return true;
	}
	void	set_socket_buf(int len)
	{
		c_socket_buf=len;
	}
	void	set_nonblock()
	{
		c_is_nonblock=true;
	}
	void	set_timeout_s(int tm)
	{
		c_timeout=tm;
	}
	void	set_max_recv_string(unsigned ms)
	{
		c_max_string=ms;
	}
	void	set_on_new_conn(void(*callback_new_conn)(Conn*))
	{
		on_new_conn=callback_new_conn;
	}
	void	set_on_close_conn(void(*callback_close_conn)(Conn*))
	{
		on_close_conn=callback_close_conn;
	}
	void	set_on_data_come(void(*callback_data_come)(Conn*))
	{
		on_data_come=callback_data_come;
	}
	bool	bind(TA&ta)
	{
		bt.tcp_addr=ta;
		if(!bt.init(ta.get_family()))
		{
			LOG_ERROR("bind socket init error:%s",ta.get_url().c_str());
			return false;
		}
		if(c_is_nonblock)
		{
			if(!bt.set_nonblock())
			{
				LOG_ERROR("bt set nonblock error");
				return false;
			}
		}

		struct epoll_event ev;
		ev.events=EPOLLIN;
		ev.data.ptr=&bt;
		if(epoll_ctl(efd,EPOLL_CTL_ADD,bt._socket_fd,&ev)==-1)
		{
			LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
			return false;
		}
		if(-1==::bind(bt._socket_fd,&bt.tcp_addr.addr.sa,sizeof(bt.tcp_addr.addr.sa)))
		{
			LOG_ERROR("bind error[%d]%s,%s",errno,strerror(errno),bt.tcp_addr.get_url().c_str());
			return false;
		}
		if(-1==::listen(bt._socket_fd,100))
		{
			LOG_ERROR("listen error[%d]%s",errno,strerror(errno));
			return false;
		}

		return true;
	}
	bool	update(int epoll_timeout_ms,int CTS,uint64 CTMS,bool*is_busy=0)
	{
		deal_with_timeout(CTS);

		int nfds=epoll_wait(efd,events,MAX_EVENTS,epoll_timeout_ms);
		if(nfds==-1)
		{
			LOG_ERROR("epoll_wait error[%d]%s",errno,strerror(errno));
			return false;
		}

		if(is_busy)	*is_busy=nfds>0?true:false;

		for(int i=0;i<nfds;i++)
		{
			if(!(events[i].events&EPOLLIN))
			{
				continue;
			}
			if(events[i].data.ptr==(void*)&bt)
			{
				Conn*hat=(Conn*)unused.get(0);
				if(!hat)
				{
					LOG_ERROR("get hat error");
					continue;
				}

				hat->tsock.tcp_addr.addrlen=sizeof(hat->tsock.tcp_addr.addr.sa);
				hat->tsock._socket_fd=accept(bt._socket_fd,&hat->tsock.tcp_addr.addr.sa,&hat->tsock.tcp_addr.addrlen);
				if(hat->tsock._socket_fd==-1)
				{
					LOG_ERROR("accept error[%d]%s",errno,strerror(errno));
					if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
					{
						LOG_ERROR("fatal error accept error");
					}
					continue;
				}
				if(c_is_nonblock)
				{
					if(!hat->tsock.set_nonblock())
					{
						LOG_ERROR("set nonblock error");
						close(hat->tsock._socket_fd);
						continue;
					}
				}
				if(!hat->tsock.set_socket_buf(c_socket_buf))
				{
					LOG_ERROR("set socket buff error");
					close(hat->tsock._socket_fd);
					continue;
				}

				struct epoll_event ev;
				ev.events=EPOLLIN;
				ev.data.ptr=hat;
				if(epoll_ctl(efd,EPOLL_CTL_ADD,hat->tsock._socket_fd,&ev)==-1)
				{
					LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
					continue;
				}
				hat->key=make_key(hat->index);
				update_conn(hat,CTS);
				active.add(hat);
				hat->udata.clear();
				hat->uptr=0;
				hat->is_timeout=false;
				if(on_new_conn)
					on_new_conn(hat);
			}
			else
			{
				Conn*conn=(Conn*)events[i].data.ptr;
				_len=::recv(conn->tsock._socket_fd,_buf,sizeof(_buf),0);
				if(_len==-1)
				{
					LOG_ERROR("recv error[%d]%s",errno,strerror(errno));
					if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
					{
						close_conn(conn);
					}
					continue;
				}
				if(_len==0)
				{
//					LOG_DEBUG("recv close req from client[%d]%s",errno,strerror(errno));
					close_conn(conn);
					continue;
				}
				update_conn(conn,CTS);
				conn->udata.append(_buf,_len);
				if(on_data_come)
					on_data_come(conn);
				if(c_max_string)
				{
					if(conn->udata.size()>c_max_string)
					{
						LOG_ERROR("recv recv string too many:%zd>%u",conn->udata.size(),c_max_string);
						close_conn(conn);
					}
				}
			}
		}
		return true;
	}
	bool	send_data(uint64 conn_key,const char*buf,int len)
	{
		Conn*conn=get_conn(conn_key);
		if(!conn)
		{
			LOG_ERROR("get connection error");
			return false;
		}

		int sl=0;
		while(sl<len)
		{
			int s;
			if((s=::send(conn->tsock._socket_fd,buf+sl,len-sl,MSG_NOSIGNAL))==-1)
			{
				LOG_ERROR("send error[%d]%s",errno,strerror(errno));
				if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
				{
					LOG_ERROR("send to error,connection should be closed after send");
//					close_conn(conn);//connection should be closed out of send, there may exists other logic before close
					return false;
				}
			}
			sl+=s;
		}
		return true;
	}
	void close_conn(Conn*conn)
	{
		if(on_close_conn)
			on_close_conn(conn);

		if(epoll_ctl(efd,EPOLL_CTL_DEL,conn->tsock._socket_fd,0)==-1)
		{
			LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
		}

		conn->clear();
		conn->just_self();
		unused.add(conn);
	}
	void close_conn(uint64 conn_key)
	{
		Conn*conn=get_conn(conn_key);
		if(conn)
			close_conn(conn);
	}
	Conn* get_conn(uint64 conn_key)
	{
		unsigned index=conn_key;
		if(index>=conn_count)
		{
			LOG_ERROR("index:%u out of bound",index);
			return 0;
		}
		Conn*conn=conn_array+index;
		if(conn->key!=conn_key)
		{
			LOG_ERROR("key mismatch, the older connection may be already closed");
			return 0;
		}
		return conn;

	}
private:
	void deal_with_timeout(int CTS)
	{
		Conn*cn=0;
		while(cn=(Conn*)active.next(0))
		{
			if(abs(CTS-cn->last_active_time)>c_timeout)
			{
				cn->is_timeout=true;
				close_conn(cn);
			}
			else
			{
				break;
			}
		}
	}
	void update_conn(Conn*conn,int CTS)
	{
		conn->last_active_time=CTS;
		active.move_to_end_as_new(conn);
	}
	uint64 make_key(unsigned i)
	{
		c_key++;
		if(c_key==0) c_key++;//conn->key=0 means it has been recyled and in unused_set
		uint64 r=c_key;
		return (r<<32)+i;
	}
};

/*****

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

*********/

}//namespace


#ifdef SELF_DEFINE_LOG
	#undef LOG_ERROR
	#undef LOG_DEBUG
	#undef SELF_DEFINE_LOG
#endif

#endif /* APPS_APOLLO_CHAT_PP_COMMON_PP_SOCKET_H_ */
