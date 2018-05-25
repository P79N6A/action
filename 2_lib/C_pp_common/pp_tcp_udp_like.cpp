/*
 * pp_tcp.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: focuszhang
 */
#include<time.h>
#include"pp_tcp.h"
namespace pp{

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
			if(port<=1000||port>65535)
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
TCPServerSocket::TCPServerSocket()
:_socket_fd(0)
,_pcliarray(0)
,_pcli_count(0)
,_efd(0)
,_has_init(false)
,_conn_unused(pp::Brick::link_use_offset)
,_conn_active(pp::Brick::link_use_offset)
{}
TCPServerSocket::~TCPServerSocket()
{
	ClientAddr*ca;
	while(ca=(ClientAddr*)_conn_active.get(0))
	{
		close_connection(ca);
	}
	close(_socket_fd);
	_socket_fd=0;
	close(_efd);
	_efd=0;
	if(_pcliarray)
	{
		delete[]_pcliarray;
		_pcliarray=0;
	}
}
bool	TCPServerSocket::init()
{
	_socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(_socket_fd==-1)
	{
		LOG_ERROR("socket create error[%d]%s",errno,strerror(errno));
		return false;
	}
	int optval=1;
	if(-1==setsockopt(_socket_fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(optval)))
	{
		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
		return false;
	}

//the following  code is not as that important
//	optval=1;
//	if(-1==setsockopt(_socket_fd,IPPROTO_TCP,TCP_NODELAY,(const void*)&optval,sizeof(optval)))
//	{
//		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
//		return false;
//	}

	//init epoll
	_efd=epoll_create(MAX_EVENTS);
	if(_efd==-1)
	{
		LOG_ERROR("epoll_create error");
		return false;
	}

	_has_init=true;
	return true;
}

bool	TCPServerSocket::bind_and_listen(const char*bind_url,unsigned listen_max)
{
	if(!_has_init)
	{
		LOG_ERROR("the socket hasn't init yet");
		return false;
	}
	if(!bind_url || !bind_url[0] || !listen_max){
		LOG_ERROR("wrong parameter");
		return false;
	}

	char bind_addr[32];
	unsigned short bind_port;
	if(!split_url(bind_url,bind_addr,&bind_port))
	{
		LOG_ERROR("split_url error:%s",bind_url);
		return false;
	}
	_local_addr.sin_family=AF_INET;
	_local_addr.sin_addr.s_addr=inet_addr(bind_addr);
	_local_addr.sin_port=htons(bind_port);
	if(-1==::bind(_socket_fd,(struct sockaddr*)&_local_addr,sizeof(struct sockaddr)))
	{
		LOG_ERROR("bind error[%d]%s,ip:%s,port:%hu",errno,strerror(errno),bind_addr,bind_port);
		return false;
	}
	if(-1==::listen(_socket_fd,listen_max))
	{
		LOG_ERROR("listen error[%d]%s",errno,strerror(errno));
		return false;
	}
	_pcli_count=listen_max;
	_pcliarray=new(std::nothrow)ClientAddr[listen_max];
	if(!_pcliarray)
	{
		LOG_ERROR("new ClientAddr error");
		return false;
	}
	for(int i=0;i<listen_max;i++)
	{
		_pcliarray[i].hash_int_key=i;
		_conn_unused.add(_pcliarray+i);
	}

	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.ptr=this;
	if(-1==epoll_ctl(_efd,EPOLL_CTL_ADD,_socket_fd,&ev))
	{
		LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
		return false;
	}
	return true;
}

void	TCPServerSocket::recv(const int action,const unsigned long long conn,const char *buf,int recv_len)
{
}
int	TCPServerSocket::update()
{

	int nfds=epoll_wait(_efd,_events,MAX_EVENTS,0);
	if(nfds==-1)
	{
		LOG_ERROR("epoll_wait error[%d]%s",errno,strerror(errno));
		return UPDATE_ERROR;
	}
	else if(nfds==0)
	{
		return UPDATE_NONE;
	}

	unsigned long long conn;
	for(int i=0;i<nfds;i++)
	{
		if(!(_events[i].events & EPOLLIN))
		{
			continue;
		}
		if(_events[i].data.ptr==this)
		{
			if(!accept_new_connection(&conn))
			{
				LOG_ERROR("accept_new_connection error");
				continue;
			}
			recv(ACTION_NEW_CONNECTION,conn,0,0);
			continue;
		}

		{
			ClientAddr*ca=(ClientAddr*)_events[i].data.ptr;
			conn=ca->hash_int_key;
			{
				int s=::recv(ca->fd,(ca->buf+ca->cur_recv_len),10240-ca->cur_recv_len,0);
				if(s==-1)
				{
					LOG_ERROR("recv error[%d]%s",errno,strerror(errno));
					if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
					{
						recv(ACTION_CLOSE_CONNECTION,conn,0,0);
						close_connection(ca);
					}
					continue;
				}
				if(s==0)
				{
					LOG_DEBUG("recv close req from client[%d]%s",errno,strerror(errno));
					recv(ACTION_CLOSE_CONNECTION,conn,0,0);
					close_connection(ca);
					continue;
				}
				ca->cur_recv_len+=s;
			}

			while(ca->cur_recv_len>=4 && ca->cur_recv_len-4>=ca->cur_msg_len)
			{
				if(!ca->cur_msg_len)
				{
					LOG_ERROR("fatal error, msg content len should not be zero, may peer set error");
					recv(ACTION_CLOSE_CONNECTION,conn,0,0);
					close_connection(ca);
					break;
				}
				if(ca->cur_msg_len>10240-4)
				{
					LOG_ERROR("recv msg too long, so close connection");
					recv(ACTION_CLOSE_CONNECTION,conn,0,0);
					close_connection(ca);
					break;
				}
				recv(ACTION_DATA_COMES,conn,ca->buf+4,ca->cur_msg_len);

				unsigned cut=ca->cur_msg_len+4;
				for(int i=0;i<ca->cur_recv_len-cut;i++)
				{
					ca->buf[i]=ca->buf[cut+i];//should not use ca->cur_msg_len for it has been changed when memcpy
				}
				ca->cur_recv_len-=cut;
			}
			continue;
		}
	}
	return UPDATE_SOMETHING_HAPPEND;
}
bool	TCPServerSocket::send(unsigned long long conn,const char*buf,unsigned len)
{
	ClientAddr*ca=0;
	if(conn)
	{
		unsigned int index=conn;
		if(index>=_pcli_count)
		{
			LOG_ERROR("wrong connection ID:%llu index:%u",conn,index);
			return false;
		}
		ca=_pcliarray+index;
		if(ca->hash_int_key!=conn)
		{
			LOG_ERROR("the connection you used[%llu] has been the history",conn);
			return false;
		}
	}
	else
	{
		ca=(ClientAddr*)_conn_active.get(0);
		if(!ca)
		{
			LOG_ERROR("the server has no aviable connection");
			return false;
		}
		_conn_active.move_to_end_as_new(ca);
	}
	if(!ca->is_connected)
	{
		LOG_ERROR("the connection:%llu has been closed",conn);
		recv(ACTION_CLOSE_CONNECTION,conn,0,0);
		return false;
	}
//send buf len first
	const char*pt=(const char*)&len;
	int sl=0;
	while(sl<4)
	{
		int s;
		if((s=::send(ca->fd,pt+sl,4-sl,MSG_NOSIGNAL))==-1)
		{
			LOG_ERROR("send error[%d]%s",errno,strerror(errno));
			if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
			{
				LOG_ERROR("close connection");
				close_connection(ca);
				return false;
			}
		}
		sl+=s;
	}
//send content
	sl=0;
	while(sl<len)
	{
		int s=::send(ca->fd,buf+sl,len-sl,MSG_NOSIGNAL);
		if(s==-1)
		{
			LOG_ERROR("send error[%d]%s",errno,strerror(errno));
			if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
			{
				LOG_ERROR("close connection");
				close_connection(ca);
				return false;
			}
		}
		sl+=s;
	}
	return true;
}
int		TCPServerSocket::get_connection_num()
{
	return _conn_active.size();
}
bool	TCPServerSocket::accept_new_connection(unsigned long long* conn)
{
	struct sockaddr_in client_addr;
	socklen_t addrlen=sizeof(client_addr);
	int new_fd=accept(_socket_fd,(struct sockaddr*)&client_addr,&addrlen);
	if(new_fd==-1)
	{
		LOG_ERROR("accept error[%d]%s",errno,strerror(errno));
		if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
		{
			LOG_ERROR("fatal error accept error");
		}
		return false;
	}

	ClientAddr*ca=(ClientAddr*)_conn_unused.get(0);
	if(!ca)
	{
		LOG_ERROR("get conn error");
		close(new_fd);
		return false;
	}

	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.ptr=ca;
	if(-1==epoll_ctl(_efd,EPOLL_CTL_ADD,new_fd,&ev))
	{
		LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
		close(new_fd);
		return false;
	}
	ca->clear();
	_conn_active.add(ca);
	do{
		ca->hash_int_key+=((unsigned long long)1<<32);
	}while(ca->hash_int_key==0);//conn should not be zero, for zero will used for empty conn when send
	ca->fd=new_fd;
	ca->addr=client_addr;
	ca->is_connected=true;

	*conn=ca->hash_int_key;
	return true;
}
void TCPServerSocket::close_connection(ClientAddr*ca)
{
	ca->is_connected=false;
	if(-1==epoll_ctl(_efd,EPOLL_CTL_DEL,ca->fd,0))
	{
		LOG_ERROR("EPOLL_CTL_DEL error[%d]%s",errno,strerror(errno));
	}
	if(-1==::close(ca->fd))
	{
		LOG_ERROR("close socket error[%d]%s",errno,strerror(errno));
	}
	ca->just_self();
	_conn_unused.add(ca);
}

void	TCPServerSocket::close_connection(unsigned long long conn)
{
	unsigned int index=conn;
	if(index>=_pcli_count)
	{
		LOG_ERROR("wrong connection ID:%llu index:%u",conn,index);
		return;
	}
	ClientAddr*ca=_pcliarray+index;
	if(ca->hash_int_key!=conn)
	{
		LOG_ERROR("the connection you used[%llu] has been the history",conn);
		return;
	}
	if(!ca->is_connected)
	{
		LOG_ERROR("the connection:%llu has been closed",conn);
		return;
	}
	close_connection(ca);
}

TCPClientSocket::TCPClientSocket()
:_last_reconenct_time(0)
,_pcliarray(0)
{}
TCPClientSocket::~TCPClientSocket()
{
	for(int i=0;i<_pcli_count;i++)
	{
		close(_pcliarray[i].fd);
	}
	close(_efd);
	if(_pcliarray)
	{
		delete[]_pcliarray;
		_pcliarray=0;
	}
}
bool	TCPClientSocket::init(unsigned send_fd_num)
{
	if(send_fd_num==0)
	{
		LOG_ERROR("send fd num at least one");
		return false;
	}
	_pcli_count=send_fd_num;
	_pcliarray=new(std::nothrow)ClientAddr[send_fd_num];
	if(!_pcliarray)
	{
		LOG_ERROR("new ClientAddr error");
		return false;
	}
	//init epoll
	_efd=epoll_create(MAX_EVENTS);
	if(_efd==-1)
	{
		LOG_ERROR("epoll_create error");
		return false;
	}
	for(int i=0;i<send_fd_num;i++)
	{
		if(!create_and_init_socket(_pcliarray+i))
		{
			LOG_ERROR("create_and_init_socket error");
			return false;
		}
	}
	return true;
}
bool	TCPClientSocket::connect(const char*url)
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
	_peer_addr.sin_addr.s_addr=inet_addr(ip);
	_peer_addr.sin_port=htons(port);

	for(int i=0;i<_pcli_count;i++)
	{

		if(-1==::connect(_pcliarray[i].fd,(struct sockaddr*)&_peer_addr,sizeof(struct sockaddr)))
		{
			LOG_ERROR("connect error[%d]%s,%s",errno,strerror(errno),url);
			return false;
		}
		_pcliarray[i].is_connected=true;
	}
	return true;
}
bool	TCPClientSocket::send(const char*buf,unsigned len)
{
	_cur_index++;
	_cur_index=(_cur_index%_pcli_count);
	ClientAddr*ca=_pcliarray+_cur_index;
	if(!ca->is_connected)
	{
		LOG_ERROR("the %lu connection has been closed, will try reconnect...",_cur_index);
		if(!reconnect(ca))
		{
			LOG_ERROR("reconnect error");
			return false;
		}
		LOG_ERROR("reconnect success");
		return false;
	}
//send buf len first
	const char*pt=(const char*)&len;
	unsigned int sl=0;
	while(sl<4)
	{
		int s;
		if((s=::send(ca->fd,pt+sl,4-sl,MSG_NOSIGNAL))==-1)
		{
			LOG_ERROR("send error[%d]%s",errno,strerror(errno));
			if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
			{
				close_connection(ca);
				return false;
			}
		}
		sl+=s;
	}
//send content
	sl=0;
	while(sl<len)
	{
		int s=::send(ca->fd,buf+sl,len-sl,MSG_NOSIGNAL);
		if(s==-1)
		{
			LOG_ERROR("send error[%d]%s",errno,strerror(errno));
			if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
			{
				close_connection(ca);
				return false;
			}
		}
		sl+=s;
	}
	return true;
}
int		TCPClientSocket::update()
{
	int nfds=epoll_wait(_efd,_events,MAX_EVENTS,0);
	if(nfds==-1)
	{
		LOG_ERROR("epoll_wait error[%d]%s",errno,strerror(errno));
		return UPDATE_ERROR;
	}
	else if(nfds==0)
	{
		return UPDATE_NONE;
	}

	for(int i=0;i<nfds;i++)
	{
		if(!(_events[i].events & EPOLLIN))
		{
			continue;
		}

		{
			ClientAddr*ca=(ClientAddr*)_events[i].data.ptr;
			{
				int s=::recv(ca->fd,(ca->buf+ca->cur_recv_len),10240-ca->cur_recv_len,0);
				if(s==-1)
				{
					LOG_ERROR("recv error[%d]%s",errno,strerror(errno));
					if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
					{
						close_connection(ca);
						continue;
					}
					continue;
				}
				if(s==0)
				{
					LOG_DEBUG("recv close req from server[%d]%s",errno,strerror(errno));
					close_connection(ca);
					//shoule not reconnect here for the server has reach it's connection limit, and reconnect will cause a const request loop
					continue;
				}
				ca->cur_recv_len+=s;
			}

			while(ca->cur_recv_len>=4 && ca->cur_recv_len-4>=ca->cur_msg_len)
			{
				if(!ca->cur_msg_len)
				{
					LOG_ERROR("fatal error, msg content len should not be zero, may peer set error");
					close_connection(ca);
					break;
				}
				if(ca->cur_msg_len>10240-4)
				{
					LOG_ERROR("recv msg too long, so close connection");
					close_connection(ca);
					break;
				}
				recv(ca->buf+4,ca->cur_msg_len);

				unsigned cut=ca->cur_msg_len+4;
				for(int i=0;i<ca->cur_recv_len-cut;i++)
				{
					ca->buf[i]=ca->buf[cut+i];//should not use ca->cur_msg_len for it has been changed when memcpy
				}
				ca->cur_recv_len-=cut;
			}
			continue;
		}
	}
	return TCPClientSocket::UPDATE_SOMETHING_HAPPEND;
}
void	TCPClientSocket::recv(const char *buf,int recv_len)
{}

bool	TCPClientSocket::create_and_init_socket(ClientAddr *ca)
{
	ca->clear();
	ca->fd=socket(AF_INET,SOCK_STREAM,0);
	if(ca->fd==-1)
	{
		LOG_ERROR("socket create error[%d]%s",errno,strerror(errno));
		return false;
	}
	LOG_DEBUG("create new fd:%d",ca->fd);
//	int optval=1;
//	if(-1==setsockopt(ca->fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(optval)))
//	{
//		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
//		return false;
//	}
//	optval=1;
//	if(-1==setsockopt(ca->fd,IPPROTO_TCP,TCP_NODELAY,(const void*)&optval,sizeof(optval)))
//	{
//		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
//		return false;
//	}
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.ptr=ca;
	if(-1==epoll_ctl(_efd,EPOLL_CTL_ADD,ca->fd,&ev))
	{
		LOG_ERROR("epollo ctl error[%d]:%s",errno,strerror(errno));
		return false;
	}
	return true;
}

void	TCPClientSocket::close_connection(ClientAddr*ca)
{
	ca->is_connected=false;
	if(-1==epoll_ctl(_efd,EPOLL_CTL_DEL,ca->fd,0))
	{
		LOG_ERROR("EPOLL_CTL_DEL error[%d]%s",errno,strerror(errno));
	}
	if(-1==close(ca->fd))
	{
		LOG_ERROR("close socet error");
	}
//	if(-1==shutdown(ca->fd,SHUT_RDWR))
//	{
//		LOG_ERROR("close socet error");
//	}
	LOG_DEBUG("close fd %d",ca->fd);
}
bool	TCPClientSocket::reconnect(ClientAddr*ca)
{
	int now=time(0);
	if(abs(_last_reconenct_time-now)<1)
	{
		LOG_ERROR("connec action interval should bigger than 1s");
		return false;
	}
	_last_reconenct_time=now;
	if(!create_and_init_socket(ca))
	{
		LOG_ERROR("create_and_init_socket error");
		return false;
	}
	if(-1==::connect(ca->fd,(struct sockaddr*)&_peer_addr,sizeof(struct sockaddr)))
	{
		unsigned char *pc=(unsigned char*)&_peer_addr.sin_addr.s_addr;
		LOG_ERROR("connect error[%d]%s,ip:%d.%d.%d.%d,port:%hu",errno,strerror(errno),pc[0],pc[1],pc[2],pc[3],htons(_peer_addr.sin_port));

		//recycle resource
		close_connection(ca);
		return false;
	}
	ca->is_connected=true;
	return true;
}
}//namespace

