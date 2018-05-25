/*
 * monitor.h
 *
 *  Created on: 2015��11��11��
 *      Author: focuszhang
 */

#ifndef MONITOR_H_PROXY
#define MONITOR_H_PROXY

#include "base_type.h"
#include "pp_socket.h"
#include "pp_time.h"
namespace mo
{
//--------------------------------------------------------------------------------------
#define STATE_ITEM(X) \
	X(list_length)	\
	X(memory_usage)	\

#define COUNT_ITEM(X) \
	X(recv)	\
	X(send)	\

//--------------------------------------------------------------------------------------


class CountItem
{
public:
	int	_count_report_interval;
	int _count_oneperiod;
	int _count_total;
public:
	CountItem()
	{
		_count_report_interval=0;
		_count_total=0;
		_count_oneperiod=0;
	}
	~CountItem(){}
	CountItem operator-(const CountItem&b)
	{
		CountItem r;
		r._count_report_interval=this->_count_report_interval-b._count_report_interval;
		r._count_oneperiod=this->_count_oneperiod-b._count_oneperiod;
		r._count_total=this->_count_total-b._count_total;
		return r;
	}
	CountItem operator+(const CountItem&b)
	{
		CountItem r;
		r._count_report_interval=this->_count_report_interval+b._count_report_interval;
		r._count_oneperiod=this->_count_oneperiod+b._count_oneperiod;
		r._count_total=this->_count_total+b._count_total;
		return r;
	}
	void add_one()
	{
		_count_report_interval++;
		_count_oneperiod++;
		_count_total++;
	}
	CountItem& operator++(int)
	{
		add_one();
		return *this;
	}
	void minus_one()
	{
		if(_count_report_interval>0)
		{
			_count_oneperiod--;
		}
		if(_count_oneperiod>0)
		{
			_count_oneperiod--;
		}
		if(_count_total>0)
		{
			_count_total--;
		}
	}
	void clear_oneperiod()
	{
		_count_oneperiod=0;
	}
	void clear_one_report_interval()
	{
		_count_report_interval=0;
	}
	void clear_all()
	{
		_count_total=0;
		_count_oneperiod=0;
		_count_report_interval=0;
	}

};


class Statistic
{
public:
#define ST(x) int x;
	STATE_ITEM(ST)
#undef ST
#define ST(x) CountItem x;
	COUNT_ITEM(ST)
#undef ST

//	char string_data[1024];
private:
	pp::UDPSocket _udp_socket;
public:
	Statistic()
	{
#define ST(x) x=0;
		STATE_ITEM(ST)
#undef ST
	}
	~Statistic()
	{
	}
	bool init(const char*url)
	{
		if(_udp_socket.init())
		{
			return _udp_socket.set_peer_addr(url);
		}
		return false;
	}
	Statistic* operator()()
	{
		return this;
	}
	void clear_oneperiod()
	{
#define STMP(x) x.clear_oneperiod();
		COUNT_ITEM(STMP)
#undef STMP
	}
	void clear_one_report_interval()
	{
#define STMP(x) x.clear_one_report_interval();
		COUNT_ITEM(STMP)
#undef STMP
	}
	void clear_all()
	{
#define STMP(x) x.clear_oneperiod();x.clear_all();
		COUNT_ITEM(STMP)
#undef STMP
#define STMP(x) x=0;
		STATE_ITEM(STMP)
#undef STMP
	}
	void clear_state_item()
	{
#define STMP(x) x=0;
		STATE_ITEM(STMP)
#undef STMP
	}
	 int run_time()
	{
		static  int start_time=time(0);
		return time(0)-start_time;
	}
	bool send_monitor(const char *content=NULL,int len=0)
	{
		static sockaddr_in peer_addr=_udp_socket._peer_addr;
		if(!content)
		{
			if(!_udp_socket._socket_is_normal)
			{
				return false;
			}
			return _udp_socket.sendto((char*)this,sizeof(Statistic),peer_addr.sin_addr.s_addr,peer_addr.sin_port);
		}

		return _udp_socket.sendto(content,len,peer_addr.sin_addr.s_addr,htons(ntohs(peer_addr.sin_port)+1));

	}
	int to_c_str(char*buf,int BUF_LEN)
	{
			int len=0;
			int item_size=0;
			item_size=snprintf(buf+len,BUF_LEN-len,"run_time\t%d\n",run_time());len+=item_size;
#define STMP(x) item_size=snprintf(buf+len,BUF_LEN-len,#x"\t%d(%d)\n",x._count_total,x._count_oneperiod);len+=item_size;
			COUNT_ITEM(STMP)
#undef STMP
#define STMP(x) item_size=snprintf(buf+len,BUF_LEN-len,#x"\t%d\n",x);len+=item_size;
			STATE_ITEM(STMP)
#undef STMP
//			len+=snprintf(buf+len,sizeof(buf)-len,"%s",string_data);
			buf[len]='\0';
			return len;
	}
	bool	send_interval_data(int now)
	{
		static int last_time=0;
		if(now-last_time>=300)
		{
			last_time=now;
			
			//do something interval

			clear_one_report_interval();
		}
		return true;
	}
};

#undef STATE_ITEM
#undef COUNT_ITEM

extern Statistic g_monitor;

}//namespace
#endif /* MONITOR_H_PROXY */
