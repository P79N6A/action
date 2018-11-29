/*
 * created by purperzhang
 * 20170919
 * jssjway@outlook.com
 */

#ifndef DETECT_ALIVE_UDP_H
#define DETECT_ALIVE_UDP_H
#include<new>
#include<map>
#include<string>
#include<vector>
#include"pp_udp.h"

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

namespace pp{

enum{
	DISASTER_INTERVAL=60,
};

struct DST_UDP
{
	UA	addr;

	const int	index;
	int	last_active_time;
	int	last_send_time;
	DST_UDP(int i):index(i){
		last_active_time=last_send_time=0;
	}
};

class SelectAliveUDP
{
	
public:
	int 	_capacity;
	DST_UDP*	_addr_array;
	int	_addr_count;
	std::map<std::string,DST_UDP*> _addr_map;
	unsigned _cur_index;
	
	SelectAliveUDP()
	{
		_cur_index=0;
		_addr_array=0;
		_addr_count=0;
	}
	virtual ~SelectAliveUDP()
	{
		if(_addr_array)
		{
			delete []_addr_array;
			_addr_array=0;
		}
	}
	bool init(int capacity)
	{
		if(capacity<=0)
		{
			LOG_ERROR("capacity can't be less or equal to zero");
			return false;
		}
		_capacity=capacity;
		_addr_array=(DST_UDP*)new(std::nothrow)char[sizeof(DST_UDP)*_capacity];
		if(!_addr_array)
		{
			LOG_ERROR("new array error");
			return false;
		}
		_cur_index=0;
		return true;
	}
	bool add(UA&ua,bool check_override=false)
	{
		if(check_override)
		{
			std::map<std::string,DST_UDP*>::iterator pos= _addr_map.find(ua.get_binary());
			if(pos!=_addr_map.end())
			{
				LOG_ERROR("check has same return false");
				return false;
			}
		}
		if(_addr_count>=_capacity)
		{
			LOG_ERROR("add exceed capacity");
			return false;
		}
		unsigned i=_addr_count;
		new(_addr_array+i)DST_UDP(i);
		_addr_array[i].last_active_time=time(0);
		_addr_array[i].last_send_time=0;

		_addr_array[i].addr=ua;
		_addr_map[_addr_array[i].addr.get_binary()]=&_addr_array[i];
		_addr_count++;
		return true;
	}
	UA*	get_loop_addr(int t=0)
	{
		if(_addr_count==0)
		{
			LOG_ERROR("not init");
			return 0;
		}
		if(t==0)
			t=time(0);
		unsigned last=_cur_index;

		DST_UDP*na=0;
		do{
			_cur_index++;
			if(_cur_index>=_addr_count)
				_cur_index=0;
			na=_addr_array+_cur_index;
			if(is_alive(t,na))
			{
				break;
			}
			LOG_ERROR("{%s} is not alive",na->addr.get_url().c_str());
			if(_cur_index==last)
			{
				LOG_ERROR("can't find alive addr,try select one");
				_cur_index++;
				if(_cur_index>=_addr_count)
					_cur_index=0;
				na=_addr_array+_cur_index;
				break;
			}
		}while(true);
		na->last_send_time=t;
		return &na->addr;
	}
	UA*	get_mode_addr(unsigned hv,int t=0)
	{
		if(_addr_count==0)
		{
			LOG_ERROR("not init");
			return 0;
		}
		if(t==0)
			t=time(0);
		unsigned index=hv%_addr_count;
		DST_UDP*na=0;
		na=_addr_array+index;
		do{
			if(is_alive(t,na))
			{
				break;
			}
			LOG_ERROR("{%s} is not alive",na->addr.get_url().c_str());
			std::vector<DST_UDP*> aa;
			for(int i=0;i<_addr_count;i++)
			{
				if(is_alive(t,_addr_array+i))
					aa.push_back(_addr_array+i);
			}
			if(aa.size()==0)
			{
				LOG_ERROR("can't find alive mode addr, returns a loop one");
				return get_loop_addr(t);
			}
			na=aa[hv%aa.size()];
		}while(false);

		na->last_send_time=t;
		return &na->addr;
	}

	bool feed_back_addr(UA&ua,int t=0)
	{
		if(t==0)
			t=time(0);
		std::map<std::string,DST_UDP*>::iterator pos=_addr_map.find(ua.get_binary());
		if(pos!=_addr_map.end())
		{
			pos->second->last_active_time=t;
			return true;
		}
		else
		{
			LOG_ERROR("can't find the addr:%s",ua.get_url().c_str());
			return false;
		}
		return true;
	}
	bool	is_alive(int t,DST_UDP*na)
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

};

}//namespace


#ifdef SELF_DEFINE_LOG
	#undef LOG_ERROR
	#undef LOG_DEBUG
	#undef SELF_DEFINE_LOG
#endif

#endif
