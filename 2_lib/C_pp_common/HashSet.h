/*
 * HashString.h
 *
 *  Created on: Sep 9, 2016
 *      Author: focuszhang
 */

#ifndef SRC_HASHSTRING_H_
#define SRC_HASHSTRING_H_
#include"focuszhang.h"
namespace pp {

class HashSet
{
public:
	enum Type{
		USE_IDNEX_0=0,
		USE_INDEX_1=1,
		USE_INDEX_2=2,
		USE_INDEX_01=3,//too int key
		USE_INDEX_012=4,//three int key
	};
	const unsigned long long _offset_type;
	Type			_key_type;
	pp::BrickSet*	_bs_array;
	unsigned int	_bs_array_count;
public:
	HashSet(unsigned long long t,Type key_type=USE_IDNEX_0);
	virtual ~HashSet();
	bool	init(unsigned int num);
	pp::Brick*	find(unsigned long long key);
	bool	insert(unsigned long long key,pp::Brick*pb);
	pp::Brick*	find(unsigned long long key1,unsigned long long key2);
	bool	insert(unsigned long long key1,unsigned long long key2,pp::Brick*pb);
	pp::Brick*	find(const char*key);
	bool	insert(const char*key,pp::Brick*pb);//the key must be value stored in the pb, use right value will cause string pointer invalid when find;
	void	move_to_begin_of_hash_node(pp::Brick*pb);
	void	move_to_end_of_hash_node(pp::Brick*pb);
private:
	int	cal_index(const char*key);
};

} /* namespace pp */

#endif /* SRC_HASHSTRING_H_ */
