/*
 * purperzhang.h
 *
 *  Created on: Aug 4, 2016
 *      Author: purperzhang
 *      Mail:	jssjway@outlook.com //contact for bugs
 */

#ifndef ROOM_MANAGER_SRC_PURPERZHANG_H_
#define ROOM_MANAGER_SRC_PURPERZHANG_H_
#include<string>
namespace pp
{
struct Link
{
	void*	pObject;
	Link*	prev;
	Link*	next;

	Link(void*po=0)
	:pObject(po)
	{
		prev=next=this;
	}
	~Link()
	{
		prev->next=next;
		next->prev=prev;
		prev=next=this;
		pObject=0;
	}
	void let_alone()
	{
		prev->next=next;
		next->prev=prev;
		prev=next=this;
		pObject=0;
	}
	void insert_prev(Link*pl)
	{
		if(pl==this) return;

		pl->next->prev=pl->prev;
		pl->prev->next=pl->next;

		pl->next=this;
		pl->prev=this->prev;

		this->prev->next=pl;
		this->prev=pl;
	}
	void insert_next(Link*pl)
	{
		if(pl==this) return;

		pl->next->prev=pl->prev;
		pl->prev->next=pl->next;

		pl->next=this->next;
		pl->prev=this;

		this->next->prev=pl;
		this->next=pl;
	}
	void move_forward()
	{
		this->prev->insert_prev(this);
	}
	void move_backward()
	{
		this->next->insert_next(this);
	}
};

class Brick
{
public:
	static unsigned long long link_use_offset;
	static unsigned long long A_offset;
	static unsigned long long B_offset;
	static unsigned long long C_offset;
	static unsigned long long D_offset;
	pp::Link	link_use;
	pp::Link	link_A;
	pp::Link	link_B;
	pp::Link	link_C;
	pp::Link	link_D;
	unsigned long long	hash_int_key[3];
	const char*		hash_string_key;
	std::string		binary_key;
public:
	Brick()
	:hash_string_key(0)
	{}
	virtual ~Brick(){}
	void	just_self();
	Link*	link(unsigned long long oft);
};
class BrickSet
{
public:
	enum{
		OK=0,
		WRONG_PARAMETER=-1,
		SET_NOT_MATCH=-2,
		ARGUMENT_IS_EQUAL=-3,
	};
	int _size;
	pp::Link	_leader;
	const unsigned long long _offset;
public:
	BrickSet(unsigned long long u)
	:_size(0)
	,_offset(u)
	{}
	virtual ~BrickSet(){}
	unsigned long long offset();
	virtual int size();
	virtual Brick*	get(Brick*pb);
	virtual Brick*	real_get(Brick*pb);
	virtual void	add(Brick*pb);
	virtual void	add_to_begin(Brick*pb);
	virtual Brick*	prev(Brick*pb);
	virtual Brick*	next(Brick*pb);
	virtual	int		insert_prev(Brick*pb0,Brick*pb1);
	virtual	int		insert_next(Brick*pb0,Brick*pb1);
	void move_forward(Brick*pb);
	void move_backward(Brick*pb);
	virtual void 	move_to_end_as_new(Brick*pb);
	virtual void 	move_to_begin(Brick*pb);
	void			release(Brick*pb);
};
}



#endif /* ROOM_MANAGER_SRC_PURPERZHANG_H_ */
