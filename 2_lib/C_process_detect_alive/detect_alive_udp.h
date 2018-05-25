/*
 * created by focuszhang
 * 20170919
 * jssjway@outlook.com
 */

#ifndef DETECT_ALIVE_UDP_H
#define DETECT_ALIVE_UDP_H
#include<new>
#include<map>
#include<string>
#include<vector>
#include"pp_socket.h"
namespace da{

enum{
	ERROR_PARAMETER=-1,
	ERROR_NEW_ARRAY=-2,
	ERROR_URL_FORMAT=-3, 
	ERROR_NO_USEFUL_ADDR=-4,
	ERROR_INDEX_OUT=-5,
	ERROR_ADD_TOO_MANY=-6,
	ERROR_CANNOT_FIND_ADDR=-7,
};
enum{
	DISASTER_INTERVAL=60,
};

struct NetAddr
{
	char	ip_str[36];
	unsigned int ip;
	unsigned short port;

	const int	index;
	int	last_active_time;
	int	last_send_time;
	NetAddr(int i):index(i){}
};

class DetectAliveUDP
{
	
public:
	int 	_capacity;
	NetAddr*	_addr_array;
	int	_addr_count;
	std::map<std::string,NetAddr*> _addr_map;
	std::map<unsigned long long,NetAddr*> _addr_map_int;
	unsigned _cur_index;
	
	DetectAliveUDP()
	{
		_cur_index=0;
		_addr_array=0;
		_addr_count=0;
	}
	virtual ~DetectAliveUDP()
	{
		if(_addr_array)
		{
			delete []_addr_array;
			_addr_array=0;
		}
	}
	int init(int capacity)
	{
		if(capacity<=0)
			return ERROR_PARAMETER;

		_capacity=capacity;
		_addr_array=(NetAddr*)new(std::nothrow)char[sizeof(NetAddr)*_capacity];
		if(!_addr_array)
		{
			return ERROR_NEW_ARRAY;
		}
		_cur_index=0;
		return 0;
	}
	int add_url(int count,const char*url,int size,bool check_override=true)
	{
		if(count<=0 || !url || size<=0)
			return ERROR_PARAMETER;
		for(int i=0;i<count;i++)
		{
			if(int r=add_url(url+size*i,check_override))
			{
				return r;
			}
		}
		return 0;
	}
	int add_url(const char*url,bool check_override=true)
	{
		if(check_override)
		{
			std::map<std::string,NetAddr*>::iterator pos= _addr_map.find(url);
			if(pos!=_addr_map.end())
				return 0;
		}
		if(_addr_count>=_capacity)
			return ERROR_ADD_TOO_MANY;

		unsigned i=_addr_count;
		new(_addr_array+i)NetAddr(i);
		_addr_array[i].last_active_time=time(0);
		_addr_array[i].last_send_time=0;

		pp::UDPSocket us;
		if(!us.set_peer_addr(url))
		{
			return ERROR_URL_FORMAT;
		}
		strncpy(_addr_array[i].ip_str,url,sizeof(_addr_array[i].ip_str));
		_addr_array[i].ip=us._peer_addr.sin_addr.s_addr;
		_addr_array[i].port=us._peer_addr.sin_port;

		_addr_map[url]=&_addr_array[i];
		_addr_map_int[cal_key(_addr_array[i].ip,_addr_array[i].port)]=&_addr_array[i];
		_addr_count++;
		return 0;
	}
	NetAddr*	get_loop_addr(int t=0)
	{
		if(_addr_count==0)
			return 0;
		if(t==0)
			t=time(0);
		unsigned last=_cur_index;

		do{
			_cur_index++;
			if(_cur_index>=_addr_count)
				_cur_index=0;
			NetAddr*na=_addr_array+_cur_index;
			if(is_alive(t,na))
			{
				na->last_send_time=t;
				return na;
			}
			if(_cur_index==last)
			{
				return 0;
			}
		}while(true);
	}
	NetAddr*	get_mode_addr(unsigned hv,int t=0)
	{
		if(_addr_count==0)
			return 0;
		if(t==0)
			t=time(0);
		unsigned index=hv%_addr_count;
		NetAddr*na=_addr_array+index;
		if(is_alive(t,na))
		{
			na->last_send_time=t;
			return na;
		}
		std::vector<NetAddr*> aa;
		for(int i=0;i<_addr_count;i++)
		{
			if(is_alive(t,_addr_array+i))
				aa.push_back(_addr_array+i);
		}
		if(aa.size()==0)
			return 0;
		na=aa[hv%aa.size()];
		na->last_send_time=t;
		return na;
	}
	int feed_back_addr(int i,int t=0)
	{
		if(t==0)
			t=time(0);
		if(i>=_addr_count)
			return ERROR_INDEX_OUT;
		_addr_array[i].last_active_time=t;
		return 0;
	}
	int feed_back_addr(unsigned int ip,unsigned short port,int t=0)
	{
		unsigned long long k=cal_key(ip,port);
		std::map<unsigned long long,NetAddr*>::iterator pos=_addr_map_int.find(k);
		if(pos!=_addr_map_int.end())
		{
			pos->second->last_active_time=t;
			return 0;
		}
		else
		{
			return ERROR_CANNOT_FIND_ADDR;
		}
	}
	bool	is_alive(int t,NetAddr*na)
	{
		if(abs(t-na->last_active_time)<=DISASTER_INTERVAL)
		{
			return true;
		}
		if(abs(t-na->last_send_time)>=(DISASTER_INTERVAL>>2))
		{
			return true;
		}
		return false;
	}
	unsigned long long cal_key(unsigned int ip,unsigned short port)
	{
		unsigned long long k=port;
		return (k<<32)+ip;
	}
};

}//namespace
#endif
