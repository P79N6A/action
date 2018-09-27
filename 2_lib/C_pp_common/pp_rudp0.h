//2016-01-04 by purperzhang


#ifndef RELIABLE_UDP_RUDP0_H_
#define RELIABLE_UDP_RUDP0_H_
#include <string>
#include "pp_socket.h"
#include "pp_deposit.h"
namespace pp
{

class RUDP0
{
public:
	enum{
		RUDP_SEQUENCE=0x1,
		RUDP_ACK=0x2,
	};
	enum{
		KEY_INDEX=0,
		FLAG_INDEX=8,
		HEAD_LEN=10,
	};
	enum{
		MSG_BUG_LEN=4096,
	};
	struct Config
	{
		int	msg_buf_len;
		int	resend_limit;
		int	resend_interval;
		int	send_deposit_capacity;
		int	recv_deposit_capacity;
		int	ack_send_num_per_time;
		Config()
		:msg_buf_len(MSG_BUG_LEN)
		,send_deposit_capacity(20480)
		,recv_deposit_capacity(204800)
		,resend_limit(4)
		,resend_interval(300)
		,ack_send_num_per_time(1)
		{}
	};
	struct Monitor
	{
		int	send_num;
		int	resend_num;
		int	data_in_num;
		int	data_true_num;
		Monitor()
		:send_num(0)
		,resend_num(0)
		,data_in_num(0)
		,data_true_num(0)
		{}
	};
	struct Record
	{
		char buf[MSG_BUG_LEN];
		int len;
		unsigned int ip;
		unsigned short port;
		int send_count;
	};
	class ResendLogic:public Deposit<Record>::ChatWithIndividual
	{
	public:
		RUDP0*	pru;
	public:
		ResendLogic(RUDP0*pr)
		:pru(pr)
		{}
		~ResendLogic(){}
		virtual  bool	chat_with_everyone(Deposit<Record>::Box*pitem)
		{
			if(pitem->item.send_count>=pru->_config.resend_limit)
			{
				if(!pru->_always_valid)
					pru->_valid=false;
				LOG_ERROR("the item with key[%llu] has send for [%d] times to ip:%s port:%hu",pitem->default_key,pitem->item.send_count,
						inet_ntoa(*reinterpret_cast<struct in_addr*>(&pitem->item.ip)),ntohs(pitem->item.port));
				pitem->parent_deposit->erase(pitem->default_key);
				pru->_prob_lost_num++;
				return true;
			}
			pitem->parent_deposit->move_to_end_as_new(pitem);
			pitem->item.send_count++;
			if(!pru->_udp_socket.sendto(pitem->item.buf,pitem->item.len,pitem->item.ip,pitem->item.port))
			{
				LOG_ERROR("resend error");
				return false;
			}
			pru->_monitor.resend_num++;
			return true;
		}
	};
	class DeleteReceiveTimeout:public Deposit<bool>::ChatWithIndividual
	{
	public:
		~DeleteReceiveTimeout(){}
		virtual  bool	chat_with_everyone(Deposit<bool>::Box*pitem)
		{
			if(!pitem->parent_deposit->erase(pitem->default_key))
			{
				LOG_ERROR("erase port_ip key item error");
				return false;
			}
			return true;
		}
	};
public:
	Config				_config;
	UDPSocket			_udp_socket;
	Deposit<Record>		_send_record;
	ResendLogic			_resend_logic;
	Deposit<bool>		_recv_record;
	DeleteReceiveTimeout	_delete_recv_record_timeout;
	bool				_valid;
	bool				_always_valid;
	int					_prob_lost_num;

	Monitor				_monitor;

public:
	RUDP0(bool al=true)
	:_resend_logic(this)
	,_valid(true)
	,_prob_lost_num(0)
	{
		_always_valid=al;
	}
	~RUDP0()
	{}
	operator bool(){return _valid;}
	Config&	get_config()
	{
		return _config;
	}
	bool init()
	{
		if(!_udp_socket.init())
		{
			LOG_ERROR("_udp_socket init failed in reliable_udp");
			return false;
		}
		if(!_send_record.init(_config.send_deposit_capacity))
		{
			LOG_ERROR("send_record init error");
			return false;
		}
		_resend_logic.chat_bound_onlyif_living_time_greater_than=_config.resend_interval;

		if(!_recv_record.init(_config.recv_deposit_capacity,_config.recv_deposit_capacity))
		{
			LOG_ERROR("recv_record init error");
			return false;
		}

		_delete_recv_record_timeout.chat_bound_onlyif_living_time_greater_than=_config.resend_interval*(_config.resend_limit+1);
		LOG_DEBUG("_delete_recv_record_timeout.chat_bound_onlyif_living_time_greater_than=%d",_delete_recv_record_timeout.chat_bound_onlyif_living_time_greater_than);
		LOG_DEBUG("rudp0_init successfully");
		return true;
	}
	bool bind_url(const char *url)
	{
		return _udp_socket.bind_url(url);
	}
	bool set_peer_addr(const char*url)
	{
		return _udp_socket.set_peer_addr(url);
	}
	bool recvfrom(char buf[],size_t buf_size,int* plen,unsigned int*cip=0,unsigned short*cport=0)
	{
		if(!_valid)
		{
			LOG_ERROR("the socket has became invalid");
			*plen=0;
			return false;
		}
		{//update
			_send_record.chat_with_individual(&_resend_logic);
			_recv_record.chat_with_individual(&_delete_recv_record_timeout);
		}
		char local_buff[MSG_BUG_LEN];
		int len;
		char dkey[15];
		unsigned int ip;
		unsigned short port;

go_on_receive:
		{//receive data
			if(!_udp_socket.recvfrom(local_buff,sizeof(local_buff),&len,&ip,&port))
			{
				LOG_ERROR("recvfrom called error in reliable_udp");
				*plen=0;
				return false;
			}
			if(len==0)
			{
				*plen=0;
				return true;
			}
			memcpy(dkey,local_buff,8);
			*(unsigned int*)(dkey+8)=ip;
			*(unsigned short*)(dkey+12)=port;
			dkey[14]='\0';
		}
		if(cip&&cport)
		{
			*cip=ip;
			*cport=port;
		}
		{
			_monitor.data_in_num++;
		}

		//deal with sequence
		if(*(unsigned short*)(local_buff+FLAG_INDEX)==RUDP_SEQUENCE)
		{
			//LOG_DEBUG("receive sequence, ");
			*(unsigned short*)(local_buff+FLAG_INDEX)=RUDP_ACK;
			for(int i=0;i<_config.ack_send_num_per_time;i++)
			{
				if(!_udp_socket.sendto(local_buff,HEAD_LEN,ip,port))
				{
					LOG_ERROR("ack send error in reliable_udp");
					*plen=0;
					return false;
				}
			}
			LOG_DEBUG("rudp0_send_ack_for_default_key:%llu",*(uint64_t*)(local_buff+KEY_INDEX));

			Deposit<bool>::Box*pbox=_recv_record.find_with_binary_key(dkey,14);
			if(!pbox)//first receive
			{
				//LOG_DEBUG("it is the first receive");
				pbox=_recv_record.get_box_with_binary_key(dkey,14);
				if(!pbox)
				{
					LOG_ERROR("recv_record get_box errro");
					*plen=0;
					return false;
				}
				memcpy(buf,local_buff+HEAD_LEN,len-HEAD_LEN);
				*plen=len-HEAD_LEN;

				_monitor.data_true_num++;
				return true;//if receive data,must submit, so can't go on receive
			}
			else
			{
				//LOG_DEBUG("it isn't the first receive");
				*plen=0;
				goto go_on_receive;
			}
		}
		//deal with ack
		else if(*(unsigned short*)(local_buff+FLAG_INDEX)==RUDP_ACK)
		{
			//LOG_DEBUG("receive ack, ");
			uint64_t default_key=*(uint64_t*)(local_buff+KEY_INDEX);
			LOG_DEBUG("rudp0_recv_ack_for_default_key:%llu",default_key);
			Deposit<Record>::Box* pbox=_send_record.find(default_key);
			if(!pbox)
			{
				//LOG_ERROR("can't find the item with key[%llu], may be it has been erased for being resend for too many times "
				//		"or maybe the first ack has been lost, and this is the second ack,"
				//		"or peer has set to send several ack",default_key);
				goto go_on_receive;//this error can be happened reasonable, so return true
			}
			if(!_send_record.erase(default_key))
			{
				LOG_ERROR("erase item error in reliable_udp");
				return false;
			}
			//LOG_DEBUG("erase the item");
			*plen=0;
			goto go_on_receive;
		}
		else
		{
			*plen=0;
			LOG_ERROR("unknown flag[%hd] udp_msg, may be being send from somewhere",*(unsigned short*)(local_buff+FLAG_INDEX));
			goto go_on_receive;//this can be happen reasonable, so return true;
		}
	}
	bool sendto(const char *buf,int len,unsigned int ip=0,unsigned short port=0)
	{
		if(len>MSG_BUG_LEN-HEAD_LEN)
		{
			LOG_ERROR("the send buffer is too big[%d>%d]",len,MSG_BUG_LEN-HEAD_LEN);
			return false;
		}
		if(!_valid)
		{
			LOG_ERROR("the reliable has became invalid");
			return false;
		}
		Deposit<Record>::Box* pbox=_send_record.get_box();
		if(!pbox)
		{
			LOG_ERROR("get box error");
			return false;
		}
		*(uint64_t*)(pbox->item.buf+KEY_INDEX)=pbox->default_key;
		*(unsigned short*)(pbox->item.buf+FLAG_INDEX)=RUDP_SEQUENCE;
		memcpy(pbox->item.buf+HEAD_LEN,buf,len);
		pbox->item.len=len+HEAD_LEN;
		pbox->item.send_count=1;
		if(ip&&port)
		{
			pbox->item.ip=ip;
			pbox->item.port=port;
		}
		else
		{
#ifdef linux
			pbox->item.ip=_udp_socket._peer_addr.sin_addr.s_addr;
			pbox->item.port=_udp_socket._peer_addr.sin_port;
#elif defined(WIN32) || defined(WIN64)
			pbox->item.ip=_udp_socket._peer_addr.sin_addr.S_un.S_addr;
			pbox->item.port=_udp_socket._peer_addr.sin_port;
#endif
		}

		_monitor.send_num++;
		LOG_DEBUG("rudp0_msg send with default_key:%llu",pbox->default_key);
		return _udp_socket.sendto(pbox->item.buf,pbox->item.len,ip,port);
	}
	std::string diagnose()
	{
		char buf[1024];
		snprintf(buf,sizeof(buf),"send_record.size():%d\trecv_record.size():%d\tprob_lost:%d\t"
				"send_num:%d\t"
				"resend_num:%d\t"
				"data_in_num:%d\t"
				"data_true_num:%d\t",
				_send_record.size(),
				_recv_record.size(),
				_prob_lost_num,
				_monitor.send_num,
				_monitor.resend_num,
				_monitor.data_in_num,
				_monitor.data_true_num);
		std::string str;
		str+=buf;
		return str;
	}
};


}//namespace



#endif /* RELIABLE_UDP_RUDP0_H_ */
