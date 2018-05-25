//2015-11-21 by focuszhang
#ifndef DEPOSIT_H
#define DEPOSIT_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include<string>
#include<stdlib.h>
#include"pp_time.h"

//#ifndef LOG_ERROR
//#define LOG_ERROR(format,...) printf("File[%s],Line[%d],Function[%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
//#endif
//#ifndef LOG_DEBUG
//#define LOG_DEBUG(format,...) printf("File[%s],Line[%d],Function[%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
//#endif
namespace pp
{
	#define DEFAULT_DEPOSIT_CAPACITY 1
	#define DEFAULT_HASH_TABLE_SIZE 0
	#define MAX_STRING_KEY_LENGTH   64

	template <typename Type>
	class Deposit
	{
	public:
		class Box;
	private:
		class Label
		{
		public:
			Label *next;
			Label *prev;
			Box *pbox;
		public:
			Label(Box*pb = NULL);
			void	insert_next(Label* plabel);
			void	insert_prev(Label* plabel);
			Label*	let_alone();
			void	move_insert_next(Label* plabel);
			void	move_insert_prev(Label* plabel);
		};
	public:
		class Box
		{
		private:
			friend class Deposit<Type>;
		public:
			uint64_t default_key;
			union
			{
				uint64_t diy_int_key;
				char diy_string_key[MAX_STRING_KEY_LENGTH];
				struct{
					char binary_key[MAX_STRING_KEY_LENGTH];
					int	 binary_len;
				}diy_binary_key;
			};
			Type item;
			Deposit<Type>* parent_deposit;
		private:
			int	 index;
			Label use_link;
			Label hash_link;
			Label chat_link;
			uint32_t create_time;
		public:
			Box();
		};
	private:
		int		_capacity;
		Box*	_parray;
		int		_hash_table_size;
		Label*	_phash_table;

		Label	_unused_list;
		Label	_used_list;

		int		_size;
	private:
		unsigned int	_unique_flag;
		uint64_t	produce_default_key_by_arrayindex(int array_index);
		int			decode_default_key_to_arrayindex(uint64_t key);
		int			produce_hash_index(uint64_t number);
		uint64_t	produce_number_by_string(const char* key, int len);
		uint32_t	gettime_ms();
	public:
		Deposit();
		~Deposit();
		bool	init(int capacity = DEFAULT_DEPOSIT_CAPACITY, int hash_table_size = DEFAULT_HASH_TABLE_SIZE);
		bool	clear();

		Box*	get_box(const Type* pitem = NULL);
		Box*	get_box_with_diykey(uint64_t key, const Type* pitem = NULL);
		Box*	get_box_with_string_key(const char* key, const Type* pitem = NULL);
		Box*	get_box_with_binary_key(const char*key,int key_len,const Type*pitem=NULL);
		Box*	insert(const Type& item);
		Box*	insert_with_diykey(uint64_t key, const Type& item);
		Box*	insert_with_string_key(const char* key, const Type& item);
		Box*	insert_with_binary_key(const char* key, int key_len, const Type& item);
		Box*	find(uint64_t default_key);
		Box*	find_with_diykey(uint64_t key);
		Box*	find_with_string_key(const char* key);
		Box*	find_with_binary_key(const char* key, int key_len);
		bool	erase(uint64_t default_key);
		bool	erase_with_diykey(uint64_t key);
		bool	erase_with_string_key(const char* key);
		bool	erase_with_binary_key(const char* key, int key_len);
		Box*	move_to_end_as_new(Box* pbox);

		bool	is_empty();
		bool	is_full();
		int		size();
		std::string	diagnose();
	private:
		int		show_list(const char*name, Label head,std::string&str);

	private:
		bool	_chat_time;
		Label	_wait_insert_list;
		Label	_wait_erase_list;
		Label	_wait_move_list;
	public:
		class ChatWithIndividual
		{
		public:
			ChatWithIndividual();
			virtual ~ChatWithIndividual();
			bool			stop_if_individual_false;
			uint32_t		chat_bound_onlyif_living_time_greater_than;
			bool			chat_through_unused_list;
			virtual  bool	chat_with_everyone(Box*pitem)=0;
		};
		bool	chat_with_individual(ChatWithIndividual*pchat_individual);
	public:
		class iterator
		{
		public:
			Label* cur_pos;
		public:
			iterator(Label*pl=0)
			:cur_pos(pl)
			{}
			Box* operator->(){return cur_pos->pbox;}
			iterator operator++(int){iterator tmp=*this;cur_pos=cur_pos->next;return tmp;}
			bool operator!=(const iterator&it)
			{
				return cur_pos!=it.cur_pos;
			}
			bool operator==(const iterator&it)
			{
				return cur_pos==it.cur_pos;
			}
		};
		iterator begin()
		{
			iterator it(&_used_list);
			it++;
			return it;
		}
		iterator end()
		{
			return iterator(&_used_list);
		}
		iterator begin_un()
		{
			iterator it(&_unused_list);
			it++;
			return it;
		}
		iterator end_un()
		{
			return iterator(&_unused_list);
		}
	};

	template <typename Type>
	Deposit<Type>::Label::Label(Box*pb)
		:next(this)
		,prev(this)
		,pbox(pb)
	{
	}

	template <typename Type>
	void Deposit<Type>::Label::insert_prev(Label *plabel)
	{
		plabel->prev = this->prev;
		plabel->next = this;

		this->prev->next = plabel;
		this->prev = plabel;
	}
	template <typename Type>
	void Deposit<Type>::Label::insert_next(Label *plabel)
	{
		plabel->next = this->next;
		plabel->prev = this;

		this->next->prev = plabel;
		this->next = plabel;
	}
	template <typename Type>
	typename Deposit<Type>::Label* Deposit<Type>::Label::let_alone()
	{
		this->prev->next = this->next;
		this->next->prev = this->prev;
		this->prev = this;
		this->next = this;
		return this;
	}
	template <typename Type>
	void Deposit<Type>::Label::move_insert_prev(Label *plabel)
	{
		plabel->let_alone();
		this->insert_prev(plabel);
	}
	template <typename Type>
	void Deposit<Type>::Label::move_insert_next(Label *plabel)
	{
		plabel->let_alone();
		this->insert_next(plabel);
	}
	//--------------------------------------------------------------------------------------------
	template <typename Type>
	Deposit<Type>::Box::Box()
		:use_link(this)
		,hash_link(this)
		,chat_link(this)
	{
		default_key = 0;
	}

	//--------------------------------------------------------------------------------------------
	template<typename Type>
	Deposit<Type>::ChatWithIndividual::ChatWithIndividual()
		:stop_if_individual_false(false)
		,chat_bound_onlyif_living_time_greater_than(0)
		,chat_through_unused_list(false)
	{}
	template<typename Type>
	Deposit<Type>::ChatWithIndividual::~ChatWithIndividual()
	{}
	//--------------------------------------------------------------------------------------------
	template <typename Type>
	uint64_t	Deposit<Type>::produce_default_key_by_arrayindex(int array_index)
	{
		_unique_flag++;
		if(_unique_flag==0)
		{//if it equals 0, may_cause the default_key=0, but many logic look zero default_key as illegal
			_unique_flag++;
		}
		uint64_t flag = _unique_flag;
		return (flag << 32) + (unsigned int)(array_index);
	}
	template <typename Type>
	int		Deposit<Type>::decode_default_key_to_arrayindex(uint64_t key)
	{
		uint64_t i = key & 0xFFFFFFFF;
		unsigned int in = static_cast<unsigned int>(i);
		return static_cast<int>(in);
	}
	template <typename Type>
	int		Deposit<Type>::produce_hash_index(uint64_t number)
	{
		return static_cast<int>(number%_hash_table_size);
	}
	template <typename Type>
	uint64_t	Deposit<Type>::produce_number_by_string(const char* key, int len)
	{
		uint64_t number = 0;
		if(len==0)
		{
			if (!key || !key[0])
			{
				LOG_ERROR("the input key is null or empty");
				return 0;
			}
			const char *p=key;
			while(*p)
			{
				number = (number << 5) + number + *p;
				p++;
			}
		}
		else
		{

			for (int i = 0; i < len; i++)
			{
				number = (number << 5) + number + key[i];
			}
		}
		return number;
	}
	
	template <typename Type>
	uint32_t	Deposit<Type>::gettime_ms()
	{
#ifdef linux
		struct timeval tv;
		gettimeofday(&tv,0);
		return tv.tv_sec*1000+tv.tv_usec/1000;
#elif defined(WIN32) || defined(WIN64)
		return GetTickCount();
#endif
	}
	template <typename Type>
	Deposit<Type>::Deposit()
		:_capacity(DEFAULT_DEPOSIT_CAPACITY)
		, _unused_list(NULL)
		, _used_list(NULL)
		, _parray(NULL)
		, _hash_table_size(DEFAULT_HASH_TABLE_SIZE)
		, _phash_table(NULL)
		, _chat_time(false)
		, _wait_insert_list(NULL)
		, _wait_erase_list(NULL)
		,_wait_move_list(NULL)
		,_size(0)
//		,_unique_flag(19900518)
	{
//		init(DEFAULT_DEPOSIT_CAPACITY, DEFAULT_HASH_TABLE_SIZE);

		_capacity = 0;
		_hash_table_size = 0;
		_size=0;
//		delete[] _parray;
//		delete[] _phash_table;
		_unused_list.next = _unused_list.prev = &_unused_list;
		_used_list.next = _used_list.prev = &_used_list;
		_chat_time = false;
		_wait_insert_list.next = _wait_insert_list.prev = &_wait_insert_list;
		_wait_erase_list.next = _wait_erase_list.prev = &_wait_erase_list;
		_wait_move_list.next=_wait_move_list.prev=&_wait_move_list;
	}
	template <typename Type>
	Deposit<Type>::~Deposit()
	{
		if(_parray)
		{
			delete[] _parray;
			_parray=NULL;
		}
		if(_phash_table)
		{
			delete[] _phash_table;
			_phash_table=NULL;
		}
	}
	template <typename Type>
	bool Deposit<Type>::init(int capacity, int hash_table_size)
	{
		if (capacity <= 0 || hash_table_size < 0)
		{
			LOG_ERROR("deposit init failed for the input argument is negative");
			return false;
		}
		_capacity = capacity;
		_hash_table_size = hash_table_size;
		_size=0;
		if(_parray)
		{
			delete[] _parray;
			_parray=NULL;
		}
		if(_phash_table)
		{
			delete[] _phash_table;
			_phash_table=NULL;
		}
		_unused_list.next = _unused_list.prev = &_unused_list;
		_used_list.next = _used_list.prev = &_used_list;
		_chat_time = false;
		_wait_insert_list.next = _wait_insert_list.prev = &_wait_insert_list;
		_wait_erase_list.next = _wait_erase_list.prev = &_wait_erase_list;
		_wait_move_list.next=_wait_move_list.prev=&_wait_move_list;

		_parray = new(std::nothrow) Box[_capacity];
		if (!_parray)
		{
			LOG_ERROR("init error in deposit");
			return false;
		}
		for (int i = 0; i < _capacity; i++)
		{
			_parray[i].index = i;
			_parray[i].parent_deposit=this;
			_unused_list.insert_prev(&_parray[i].use_link);
		}
		if (_hash_table_size)
		{
			_phash_table = new(std::nothrow)  Label[_hash_table_size];
			if (!_phash_table)
			{
				LOG_ERROR("new hash table failed");
				return false;
			}
		}
		srand(time(0));
		_unique_flag=rand()+_capacity+_hash_table_size;
		return true;
	}

	template <typename Type>
	bool	Deposit<Type>::clear()
	{
		if(_chat_time)
		{
			LOG_ERROR("clear can only be called out of chat_time");
			return false;
		}
		_size=0;

		if(_hash_table_size)
		{
			Label* plabel=_used_list.next;
			while(plabel)
			{
				plabel->pbox->hash_link.let_alone();
			}
		}
		_unused_list.prev->next=_used_list.next;
		_used_list.next->prev=_unused_list.prev;
		_unused_list.prev=_used_list.prev;
		_used_list.prev->next=&_unused_list;
		_used_list.let_alone();
		return true;
	}

	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::get_box(const Type* pitem)
	{
		if (_unused_list.next == &_unused_list)
		{
			LOG_ERROR("the deposit has no unused box(is full), can't insert");
			return NULL;
		}
		Label* plabel = _unused_list.next->let_alone();
		if (pitem)
		{
			plabel->pbox->item = *pitem;
		}
		plabel->pbox->default_key = produce_default_key_by_arrayindex(plabel->pbox->index);
		if(plabel->pbox->default_key==0)
		{
			LOG_ERROR("the default_key should never be zero_431");
			return NULL;
		}
		plabel->pbox->create_time = gettime_ms();

		if (!_chat_time)
		{
			_used_list.insert_prev(plabel);
			_size++;
		}
		else
		{
			_wait_insert_list.insert_prev(&plabel->pbox->chat_link);
		}

		return plabel->pbox;
	}

	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::get_box_with_diykey(uint64_t key, const Type* pitem)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.insert");
			return NULL;
		}
		if (find_with_diykey(key))
		{
			LOG_ERROR("the item with key[%llu] already exist, so can't insert", key);
			return NULL;
		}
		if (_unused_list.next == &_unused_list)
		{
			LOG_ERROR("the deposit has no unused box(is full), can't insert");
			return NULL;
		}

		Label* plabel = _unused_list.next->let_alone();
		if (pitem)
		{
			plabel->pbox->item = *pitem;
		}
		plabel->pbox->default_key = produce_default_key_by_arrayindex(plabel->pbox->index);
		plabel->pbox->diy_int_key = key;
		plabel->pbox->create_time = gettime_ms();


		if (!_chat_time)
		{
			_used_list.insert_prev(plabel);
			_size++;
		}
		else
		{
			_wait_insert_list.insert_prev(&plabel->pbox->chat_link);
		}
		_phash_table[produce_hash_index(key)].insert_prev(&plabel->pbox->hash_link);

		return plabel->pbox;
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::get_box_with_string_key(const char* key, const Type* pitem)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init or the set value is zero, so can only use defalut key by calling Deposit.insert");
			return NULL;
		}
		if (!key || key[0] == '\0')
		{
			LOG_ERROR("the string pointer is null or the string is null");
			return NULL;
		}
		if (find_with_string_key(key))
		{
			LOG_ERROR("the item with key[%s] already exist, so can't insert", key);
			return NULL;
		}
		if (_unused_list.next == &_unused_list)
		{
			LOG_ERROR("the deposit has no unused box(is full), can't insert");
			return NULL;
		}

		Label* plabel = _unused_list.next->let_alone();
		if (pitem)
		{
			plabel->pbox->item = *pitem;
		}
		plabel->pbox->default_key = produce_default_key_by_arrayindex(plabel->pbox->index);
		strncpy(plabel->pbox->diy_string_key, key, MAX_STRING_KEY_LENGTH);
		plabel->pbox->create_time = gettime_ms();

		if (!_chat_time)
		{
			_used_list.insert_prev(plabel);
			_size++;
		}
		else
		{
			_wait_insert_list.insert_prev(&plabel->pbox->chat_link);
		}
		_phash_table[produce_hash_index(produce_number_by_string(key, 0))].insert_prev(&plabel->pbox->hash_link);

		return plabel->pbox;
	}

	template <typename Type>
	typename Deposit<Type>::Box*	Deposit<Type>::get_box_with_binary_key(const char*key,int key_len,const Type*pitem)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init or the set value is zero, so can only use defalut key by calling Deposit.insert");
			return NULL;
		}
		if (find_with_binary_key(key,key_len))
		{
			LOG_ERROR("the item with key[%s] already exist, so can't insert", key);
			return NULL;
		}
		if (_unused_list.next == &_unused_list)
		{
			LOG_ERROR("the deposit has no unused box(is full), can't insert");
			return NULL;
		}

		Label* plabel = _unused_list.next->let_alone();
		if (pitem)
		{
			plabel->pbox->item = *pitem;
		}
		plabel->pbox->default_key = produce_default_key_by_arrayindex(plabel->pbox->index);
		memcpy(plabel->pbox->diy_binary_key.binary_key,key,key_len);
		plabel->pbox->diy_binary_key.binary_len=key_len;
		plabel->pbox->create_time = gettime_ms();

		if (!_chat_time)
		{
			_used_list.insert_prev(plabel);
			_size++;
		}
		else
		{
			_wait_insert_list.insert_prev(&plabel->pbox->chat_link);
		}
		_phash_table[produce_hash_index(produce_number_by_string(key, key_len))].insert_prev(&plabel->pbox->hash_link);

		return plabel->pbox;
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::insert(const Type& item)
	{
		return get_box(&item);
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::insert_with_diykey(uint64_t key, const Type& item)
	{
		return get_box_with_diykey(key, &item);
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::insert_with_string_key(const char* key, const Type& item)
	{
		return get_box_with_string_key(key, &item);
	}

	template <typename Type>
	typename Deposit<Type>::Box*	Deposit<Type>::insert_with_binary_key(const char* key, int key_len, const Type& item)
	{
		return get_box_with_binary_key(key, key_len, &item);
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::find(uint64_t default_key)
	{
		if (!default_key)
		{
			LOG_ERROR("the input default_key is error for the default_key must be nonzero");
			return NULL;
		}
		int index = decode_default_key_to_arrayindex(default_key);
		if(index<0 || index>=_capacity)
		{
			LOG_ERROR("decode index error");
			return NULL;
		}
		if (_parray[index].default_key == default_key)
		{
			return _parray + index;
		}
		else
		{
			return NULL;
		}
	}
	template <typename Type>
	typename Deposit<Type>::Box* Deposit<Type>::find_with_diykey(uint64_t key)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.find");
			return false;
		}
		int index = produce_hash_index(key);
		Label* p = _phash_table[index].next;
		while (p->pbox)
		{
			if (p->pbox->diy_int_key == key)
			{
				return p->pbox;
			}
			else
			{
				p = p->next;
			}
		}
		return NULL;
	}
	template <typename Type>
	typename Deposit<Type>::Box*	Deposit<Type>::find_with_string_key(const char* key)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.find");
			return NULL;
		}
		if (!key || key[0] == '\0')
		{
			LOG_ERROR("the string key is null or the pointer is null");
			return NULL;
		}
		int index = produce_hash_index(produce_number_by_string(key, 0));
		Label* p = _phash_table[index].next;
		while (p->pbox)
		{
			if (strncmp(p->pbox->diy_string_key, key, MAX_STRING_KEY_LENGTH) == 0)
			{
				return p->pbox;
			}
			else
			{
				p = p->next;
			}
		}
		return NULL;
	}

	template <typename Type>
	typename Deposit<Type>::Box*	Deposit<Type>::find_with_binary_key(const char* key, int key_len)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.find");
			return NULL;
		}
		int index = produce_hash_index(produce_number_by_string(key, key_len));
		Label* p = _phash_table[index].next;
		while (p->pbox)
		{
			if(key_len!=p->pbox->diy_binary_key.binary_len)
			{
				LOG_ERROR("the binary key length is not match");
				return NULL;
			}
			bool r=true;
			for(int i=0;i<key_len;i++)
			{
				if(p->pbox->diy_binary_key.binary_key[i]!=key[i])
				{
					r=false;
					break;
				}
			}
			if (r)
			{
				return p->pbox;
			}
			else
			{
				p = p->next;
			}
		}
		return NULL;
	}
	template <typename Type>
	bool	Deposit<Type>::erase(uint64_t default_key)
	{
		if (!default_key)
		{
			LOG_ERROR("the input default_key is error for the default_key must be nonzero");
			return false;
		}
		int index = decode_default_key_to_arrayindex(default_key);
		if(index<0 || index>=_capacity)
		{
			LOG_ERROR("decode index error");
			return false;
		}
		if (_parray[index].default_key == default_key)
		{
			_parray[index].default_key = 0;
			if (!_chat_time)
			{
				_unused_list.move_insert_prev(&_parray[index].use_link);
				_size--;
			}
			else
			{
				_wait_erase_list.move_insert_prev(&_parray[index].chat_link);
			}
			if(_hash_table_size)
			{
				_parray[index].hash_link.let_alone();
			}
			return true;
		}
		else
		{
			LOG_ERROR("the key[%llu] has been released", default_key);
			return false;
		}
	}
	template <typename Type>
	bool	Deposit<Type>::erase_with_diykey(uint64_t key)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.release");
			return false;
		}
		int index = produce_hash_index(key);
		Label* plabel = _phash_table[index].next;
		while (plabel->pbox)
		{
			if (plabel->pbox->diy_int_key == key)
			{
				plabel->pbox->default_key = 0;
				plabel->pbox->diy_int_key = 0;
				if (!_chat_time)
				{
					_unused_list.move_insert_prev(&plabel->pbox->use_link);
					_size--;
				}
				else
				{
					_wait_erase_list.move_insert_prev(&plabel->pbox->chat_link);
				}
				plabel->let_alone();
				return true;
			}
			else
			{
				plabel = plabel->next;
			}
		}
		return false;
	}
	template <typename Type>
	bool	Deposit<Type>::erase_with_string_key(const char* key)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.release");
			return false;
		}
		if (!key || key[0] == '\0')
		{
			LOG_ERROR("the string is null or the pointer is null");
			return false;
		}
		int index = produce_hash_index(produce_number_by_string(key, 0));
		Label* plabel = _phash_table[index].next;
		while (plabel->pbox)
		{
			if (strncmp(plabel->pbox->diy_string_key, key, MAX_STRING_KEY_LENGTH) == 0)
			{
				plabel->pbox->default_key = 0;
				plabel->pbox->diy_string_key[0] = '\0';
				if (!_chat_time)
				{
					_unused_list.move_insert_prev(&plabel->pbox->use_link);
					_size--;
				}
				else
				{
					_wait_erase_list.move_insert_prev(&plabel->pbox->chat_link);
				}
				plabel->let_alone();
				return true;
			}
			else
			{
				plabel = plabel->next;
			}
		}
		return false;
	}

	template <typename Type>
	bool	Deposit<Type>::erase_with_binary_key(const char* key, int key_len)
	{
		if (!_hash_table_size)
		{
			LOG_ERROR("the hash table hasn't been set in init, so can only use defalut key by calling Deposit.find");
			return false;
		}
		int index = produce_hash_index(produce_number_by_string(key, key_len));
		Label* p = _phash_table[index].next;
		while (p->pbox)
		{
			if(key_len!=p->pbox->diy_binary_key.binary_len)
			{
				LOG_ERROR("the binary key length is not match");
				return false;
			}
			bool r=true;
			for(int i=0;i<key_len;i++)
			{
				if(p->pbox->diy_binary_key.binary_key[i]!=key[i])
				{
					r=false;
					break;
				}
			}
			if (r)
			{
				p->pbox->default_key = 0;
				p->pbox->diy_binary_key.binary_key[0] = '\0';
				p->pbox->diy_binary_key.binary_len=0;
				if (!_chat_time)
				{
					_unused_list.move_insert_prev(&p->pbox->use_link);
					_size--;
				}
				else
				{
					_wait_erase_list.move_insert_prev(&p->pbox->chat_link);
				}
				p->let_alone();
				return true;
			}
			else
			{
				p = p->next;
			}
		}
		return false;
	}
	template <typename Type>
	typename Deposit<Type>::Box*	Deposit<Type>::move_to_end_as_new(Box* pbox)
	{
		if(!pbox)
		{
			LOG_ERROR("the input pointer is null");
			return NULL;
		}
		pbox->create_time=gettime_ms();
		if(!_chat_time)
		{
			_used_list.move_insert_prev(&pbox->use_link);
		}
		else
		{
			_wait_move_list.move_insert_prev(&pbox->chat_link);
		}
		return pbox;
	}

	template <typename Type>
	bool	Deposit<Type>::is_empty()
	{
		return _used_list.next == &_used_list;
	}
	template <typename Type>
	bool	Deposit<Type>::is_full()
	{
		return _unused_list.next == &_unused_list;
	}

	template <typename Type>
	int		Deposit<Type>::size()
	{
		return _size;
	}
	template <typename Type>
	bool	Deposit<Type>::chat_with_individual(ChatWithIndividual*pchat_with)
	{
		if (!pchat_with)
		{
			LOG_ERROR("the input pointer is null");
			return false;
		}
		if(pchat_with->chat_through_unused_list)
		{
			bool ret = true;
			Label* plabel = _unused_list.next;
			while(plabel->pbox)
			{
				if (!pchat_with->chat_with_everyone(plabel->pbox))
				{
					ret = false;
				}
				if (pchat_with->stop_if_individual_false && !ret)
				{
					break;
				}
				ret = true;
				plabel = plabel->next;
			}
			return ret;
		}
		bool ret = true;
		Label* plabel = _used_list.next;
		uint32_t now = gettime_ms();
		_chat_time = true;
		while (plabel->pbox)
		{
			if (abs(int(now - plabel->pbox->create_time)) < abs(int(pchat_with->chat_bound_onlyif_living_time_greater_than)))
			{
				break;
			}
			if (!pchat_with->chat_with_everyone(plabel->pbox))
			{
				ret = false;
			}
			if (pchat_with->stop_if_individual_false && !ret)
			{
				break;
			}
			ret = true;
			plabel = plabel->next;
		}
		_chat_time = false;
		{
			plabel = _wait_insert_list.next;
			while (plabel->pbox)
			{
				_used_list.move_insert_prev(&plabel->pbox->use_link);
				_size++;
				Label* ptmp = plabel;
				plabel = plabel->next;
				ptmp->let_alone();
			}

			plabel = _wait_erase_list.next;
			while (plabel->pbox)
			{
				_unused_list.move_insert_prev(&plabel->pbox->use_link);
				_size--;
				Label*ptmp = plabel;
				plabel = plabel->next;
				ptmp->let_alone();
			}

			plabel=_wait_move_list.next;
			while (plabel->pbox)
			{
				_used_list.move_insert_prev(&plabel->pbox->use_link);
				Label*ptmp = plabel;
				plabel = plabel->next;
				ptmp->let_alone();
			}
		}
		return ret;
	}
	template<typename Type>
	int Deposit<Type>::show_list(const char*name, Label head,std::string&str)
	{
		char buf[1024];
		snprintf(buf,sizeof(buf),"%s\t", name);
		str+=buf;

		int n = 0;
		Label*plabel = head.next;
		while (plabel->pbox)
		{
			n++;
			snprintf(buf,sizeof(buf),"%d ", plabel->pbox->index);
			str+=buf;
			plabel = plabel->next;
		}
		snprintf(buf,sizeof(buf),"\ttotal:%d\n", n);
		str+=buf;
		return n;
	}
	template <typename Type>
	std::string	Deposit<Type>::diagnose()
	{
		std::string str;
		char buf[1024];
		str+="{--------->DIAGNOSE\n";
		if (_used_list.pbox)
		{
			LOG_ERROR("the _used_list head is unnormal");
			return false;
		}
		if (_unused_list.pbox)
		{
			LOG_ERROR("the _unused_list head is unnormal");
			return false;
		}
		show_list("unused_list=>", _unused_list,str);
		show_list("used_list=>", _used_list,str);
		show_list("wait_insert_list=>", _wait_insert_list,str);
		show_list("wait_erase_list=>", _wait_erase_list,str);


		str+="hash_table=>\n";
		int N = 0;
		for (int i = 0; i < _hash_table_size; i++)
		{
			char buf[100];
#ifdef linux
			snprintf(buf, sizeof(buf), "hash_table[%d]\t", i);
#elif defined(WIN32) || defined(WIN64)
			sprintf(buf, "hash_table[%d]\t", i);
#endif
			N += show_list(buf, _phash_table[i],str);
		}
		snprintf(buf,sizeof(buf),"hash total:%d", N);

		str+=("}<--------DIAGNOSE\n");
		return str;
	}

}


#endif // !DEPOSIT_H
