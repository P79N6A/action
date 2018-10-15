/*
 * HashString.cpp
 *
 *  Created on: Sep 9, 2016
 *      Author: purperzhang
 */
#include<new>
#include<string.h>
#include "HashSet.h"

#ifndef LOG_ERROR
#define SELF_DEFINE_LOG_ERROR
#define LOG_ERROR(format,args...)
#endif

namespace pp {

inline unsigned long long mixint(unsigned long long k1,unsigned long long k2)
{
	return ((k1+k2)*(k1+k2+1)>>2)+k2;
}

HashSet::HashSet(unsigned long long t,Type key_type)
:_offset_type(t)
,_key_type(key_type)
,_bs_array(0)
,_bs_array_count(0)
,_max_hair(0)
{
	// TODO Auto-generated constructor stub

}

HashSet::~HashSet() {
	// TODO Auto-generated destructor stub
	if(_bs_array)
	{
		delete[]_bs_array;
		_bs_array=0;
	}
}

bool	HashSet::init(unsigned int num)
{
	if(num==0)
	{
		LOG_ERROR("num can't be zero");
		return false;
	}
	_bs_array_count=num;
	_bs_array=(pp::BrickSet*)new(std::nothrow)char[num*sizeof(pp::BrickSet)];
	if(!_bs_array)
	{
		LOG_ERROR("new BrickSet error");
		return false;
	}
	for(int i=0;i<num;i++)
	{
		new(_bs_array+i)pp::BrickSet(_offset_type);
	}
	return true;
}
pp::Brick*	HashSet::find(unsigned long long key)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return 0;
	}
	if(_key_type>=USE_INDEX_01)
		return 0;

	unsigned long long  index=(key%_bs_array_count);
	pp::BrickSet*pbs=_bs_array+index;
	pp::Brick*pb=0;
	while(pb=pbs->next(pb))
	{
		if(key==pb->hash_int_key[_key_type])
		{
			return pb;
		}
	}
	return 0;
}
bool	HashSet::insert(unsigned long long key,pp::Brick*pb)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return false;
	}
	if(_key_type>=USE_INDEX_01)
		return false;

	unsigned long long index=(key%_bs_array_count);
	_bs_array[index].add(pb);
	pb->hash_int_key[_key_type]=key;
	if(_bs_array[index].size()>_max_hair)
	{
		_max_hair=_bs_array[index].size();
	}
	return true;
}
pp::Brick*	HashSet::find(unsigned long long key0,unsigned long long key1)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return 0;
	}
	if(_key_type!=USE_INDEX_01)
	{
		LOG_ERROR("_key_type mismatch");
		return 0;
	}

	unsigned long long  index=(mixint(key0,key1)%_bs_array_count);
	pp::BrickSet*pbs=_bs_array+index;
	pp::Brick*pb=0;
	while(pb=pbs->next(pb))
	{
		if(key0==pb->hash_int_key[0] && key1==pb->hash_int_key[1])
		{
			return pb;
		}
	}
	return 0;
}
bool	HashSet::insert(unsigned long long key0,unsigned long long key1,pp::Brick*pb)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return false;
	}
	if(_key_type!=USE_INDEX_01)
	{
		return false;
	}

	unsigned long long index=(mixint(key0,key1)%_bs_array_count);
	_bs_array[index].add(pb);
	pb->hash_int_key[0]=key0;
	pb->hash_int_key[1]=key1;
	if(_bs_array[index].size()>_max_hair)
	{
		_max_hair=_bs_array[index].size();
	}
	return true;
}
pp::Brick*	HashSet::find(const char*key)
{
	if(_bs_array_count==0)
	{
//		LOG_ERROR("the hash set has not been init");
		return 0;
	}
	if(!key)
	{
		return 0;
	}

	unsigned  index=cal_index(key);
	pp::BrickSet*pbs=_bs_array+index;
	pp::Brick*pb=0;
	while(pb=pbs->next(pb))
	{
		if(strcmp(key,pb->hash_string_key)==0)
		{
			return pb;
		}
	}
	return 0;
}
bool	HashSet::insert(const char*key,pp::Brick*pb)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return false;
	}

	unsigned index=cal_index(key);
	_bs_array[index].add(pb);
	pb->hash_string_key=key;
	if(_bs_array[index].size()>_max_hair)
	{
		_max_hair=_bs_array[index].size();
	}
	return true;
}

pp::Brick*	HashSet::find(const std::string key)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return 0;
	}
	if(_key_type!=USE_INDEX_BINARY)
	{
		LOG_ERROR("key type misgmatch");
		return 0;
	}
	unsigned index=cal_index(key.c_str(),key.size());
	pp::BrickSet*pbs=_bs_array+index;
	pp::Brick*pb=0;
	while(pb=pbs->next(pb))
	{
		if(key==pb->binary_key)
		{
			return pb;
		}
	}
	return 0;
}
bool	HashSet::insert(const std::string key,pp::Brick*pb)
{
	if(_bs_array_count==0)
	{
		LOG_ERROR("the hash set has not been init");
		return false;
	}
	if(_key_type!=USE_INDEX_BINARY)
	{
		LOG_ERROR("key type misgmatch");
		return false;
	}

	unsigned index=cal_index(key.c_str(),key.size());
	_bs_array[index].add(pb);
	pb->binary_key=key;
	if(_bs_array[index].size()>_max_hair)
	{
		_max_hair=_bs_array[index].size();
	}
	return true;
}
pp::Brick*	HashSet::dfind(unsigned long long hash,const void*stand,bool (*equal_judge)(const void*, const void*))
{
	if(_key_type!=USE_DIRECT)
	{
		LOG_ERROR("key type misgmatch");
		return 0;
	}
	unsigned index=hash%_bs_array_count;
	pp::BrickSet*pbs=_bs_array+index;
	pp::Brick*pb=0;
	while(pb=pbs->next(pb))
	{
		if(equal_judge(stand,pb))
		{
			return pb;
		}
	}
	return 0;
}
bool	HashSet::dinsert(unsigned long long hash,pp::Brick*pb)
{
	if(_key_type!=USE_DIRECT)
	{
		LOG_ERROR("key type misgmatch");
		return false;
	}
	unsigned index=hash%_bs_array_count;
	_bs_array[index].add(pb);
	if(_bs_array[index].size()>_max_hair)
	{
		_max_hair=_bs_array[index].size();
	}
	return true;
}
int	HashSet::cal_index(const char*key,unsigned len)
{
	unsigned v=0;
	const char*p=key;
	if(len==0)
	{
		while(*p)
		{
			v=(v<<5)+v+*p;
			p++;
		}
	}
	else
	{
		unsigned i=0;
		while(i<len)
		{
			v=(v<<5)+v+*(key+i);
			i++;
		}
	}
	return v%_bs_array_count;
}
void	HashSet::move_to_begin_of_hash_node(pp::Brick*pb)
{
	((pp::BrickSet*)pb->link(_offset_type)->pObject)->move_to_begin(pb);
}
void	HashSet::move_to_end_of_hash_node(pp::Brick*pb)
{
	((pp::BrickSet*)pb->link(_offset_type)->pObject)->move_to_end_as_new(pb);
}
} /* namespace sv */

#ifdef SELF_DEFINE_LOG_ERROR
#undef SELF_DEFINE_LOG_ERROR
#undef LOG_ERROR
#endif
