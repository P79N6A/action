//20161201 by focuszhang


#ifndef PP_COMMON_PP_TCP_H_
#define PP_COMMON_PP_TCP_H_

#ifdef linux
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <winsock2.h>
//#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <new>
#include"focuszhang.h"
#include"log.h"
namespace pp
{
//
bool split_url(const char*url,char ip[],unsigned short*pport);
class ClientAddr;

class TCPServerSocket
{
public:
	enum{
		UPDATE_NONE,
		UPDATE_ERROR,
		UPDATE_SOMETHING_HAPPEND,
		ACTION_NEW_CONNECTION,
		ACTION_DATA_COMES,
		ACTION_CLOSE_CONNECTION,
		MAX_EVENTS=10000,
	};
private:
	int					_socket_fd;
	struct sockaddr_in	_local_addr;

	int	_efd;
	struct epoll_event _events[MAX_EVENTS];

	ClientAddr*	_pcliarray;
	unsigned	_pcli_count;
	pp::BrickSet	_conn_unused;
	pp::BrickSet	_conn_active;

	bool		_has_init;
public:
	TCPServerSocket();
	virtual	~TCPServerSocket();
	bool			init();
	bool			bind_and_listen(const char*bind_url,unsigned listen_max);
	int				update();
	virtual	void	recv(const int action,const unsigned long long conn,const char *buf,int recv_len);
	bool			send(unsigned long long conn,const char*buf,unsigned len);
	void			close_connection(unsigned long long conn);
	int				get_connection_num();
private:
	bool		accept_new_connection(unsigned long long* conn);
	void 		close_connection(ClientAddr*ca);
};

class TCPClientSocket
{
public:
	enum{
		UPDATE_NONE,
		UPDATE_ERROR,
		UPDATE_SOMETHING_HAPPEND,
		MAX_EVENTS=1000,
	};
private:
	struct sockaddr_in _peer_addr;

	int	_efd;
	struct epoll_event _events[MAX_EVENTS];

	ClientAddr*		_pcliarray;
	unsigned		_pcli_count;

	unsigned int	_cur_index;
	int				_last_reconenct_time;
public:
	TCPClientSocket();
	virtual	~TCPClientSocket();
	bool			init(unsigned send_fd_num);
	bool			connect(const char*url);
	bool			send(const char*buf,unsigned len);
	int				update();
	virtual	void	recv(const char *buf,int recv_len);
private:
	bool	create_and_init_socket(ClientAddr*ca);
	void	close_connection(ClientAddr*ca);
	bool	reconnect(ClientAddr*ca);
};

class ClientAddr:public pp::Brick
{
public:
	int	fd;
	struct sockaddr_in	addr;

	unsigned&	cur_msg_len;
	unsigned	cur_recv_len;
	char		buf[10240];

	bool		is_connected;
	ClientAddr()
	:cur_msg_len(*(unsigned*)buf)
	,is_connected(false)
	{}
	void clear()
	{
		is_connected=false;
		fd=0;
		*(int*)buf=0;
		cur_recv_len=0;
	}
};



}//namespace



#endif /* PP_COMMON_PP_TCP_H_ */
