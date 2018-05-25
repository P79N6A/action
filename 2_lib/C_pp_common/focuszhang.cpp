/*
 * focuszhang.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: focuszhang
 *      Mail:	jssjway@outlook.com //contact for bugs
 */
#include"focuszhang.h"
namespace pp
{

void* ZERO=0;
unsigned long long Brick::link_use_offset=(unsigned long long)(&((Brick*)ZERO)->link_use);
unsigned long long Brick::A_offset=(unsigned long long)(&((Brick*)ZERO)->link_A);
unsigned long long Brick::B_offset=(unsigned long long)(&((Brick*)ZERO)->link_B);
unsigned long long Brick::C_offset=(unsigned long long)(&((Brick*)ZERO)->link_C);
unsigned long long Brick::D_offset=(unsigned long long)(&((Brick*)ZERO)->link_D);

void	Brick::just_self()
{
//	if(link_use.pObject)
//		((BrickSet*)link_use.pObject)->_size--;
	if(link_A.pObject)
		((BrickSet*)link_A.pObject)->_size--;
	if(link_B.pObject)
		((BrickSet*)link_B.pObject)->_size--;
	if(link_C.pObject)
		((BrickSet*)link_C.pObject)->_size--;
	if(link_D.pObject)
		((BrickSet*)link_D.pObject)->_size--;

//	link_use.let_alone();
	link_A.let_alone();
	link_B.let_alone();
	link_C.let_alone();
	link_D.let_alone();
}
Link*	Brick::link(unsigned long long oft)
{
	return (pp::Link*)((char*)this+oft);
}

inline unsigned long long BrickSet::offset()
{
	return _offset;
}
inline int BrickSet::size()
{
	return _size;
}
Brick*	BrickSet::get(Brick*pb)
{
	if(_leader.next==&_leader)
	{
		return 0;
	}
	if(pb==0)
	{
		pp::Link*pl=_leader.next;
		if(_offset!=Brick::link_use_offset)
		{
			pl->let_alone();
			_size--;
		}
		return (Brick*)((char*)pl-_offset);
	}
	else
	{
		pp::Link*pl=(pp::Link*)((char*)pb+_offset);
		if(pl->pObject!=this)
		{
			return 0;
		}
		if(_offset!=Brick::link_use_offset)
		{
			this->_size--;
			pl->let_alone();
		}
		return pb;
	}
}
void	BrickSet::add(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(pl->pObject)
		((BrickSet*)pl->pObject)->_size--;
	pl->pObject=this;
	_leader.insert_prev(pl);
	_size++;
}
void	BrickSet::add_to_begin(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(pl->pObject)
		((BrickSet*)pl->pObject)->_size--;
	pl->pObject=this;
	_leader.insert_next(pl);
	_size++;
}
Brick*	BrickSet::prev(Brick*pb)
{
	pp::Link*pl;
	if(pb==0)
	{
		pl=&_leader;
	}
	else
	{
		pl=(pp::Link*)((char*)pb+_offset);
		if(pl->pObject!=this)
		{
			return 0;
		}
	}
	pl=pl->prev;
	if(pl==&_leader)
	{
		return 0;
	}
	return (Brick*)((char*)pl-_offset);
}
Brick*	BrickSet::next(Brick*pb)
{
	pp::Link*pl;
	if(pb==0)
	{
		pl=&_leader;
	}
	else
	{
		pl=(pp::Link*)((char*)pb+_offset);
		if(pl->pObject!=this)
		{
			return 0;
		}
	}
	pl=pl->next;
	if(pl==&_leader)
	{
		return 0;
	}
	return (Brick*)((char*)pl-_offset);
}

int	BrickSet::insert_prev(Brick*pb0,Brick*pb1)
{
	if(!pb1)	return WRONG_PARAMETER;//wrong parameter
	if(pb0==pb1)	return ARGUMENT_IS_EQUAL;
	pp::Link* pl1=(pp::Link*)((char*)pb1+_offset);
	if(pb0==0)
	{
		if(pl1->pObject!=this)
			this->add(pb1);
		else if(_leader.prev!=pl1)
			_leader.insert_prev(pl1);
	}
	else
	{
		pp::Link* pl0=(pp::Link*)((char*)pb0+_offset);
		if(pl0->pObject!=this)
			return SET_NOT_MATCH;
		if(pl1->pObject!=this)
		{
			if(pl1->pObject)
				((BrickSet*)pl1->pObject)->_size--;
			pl1->pObject=this;
			pl0->insert_prev(pl1);
			_size++;
		}
		else
		{
			pl0->insert_prev(pl1);
		}
	}
	return 0;
}
int	BrickSet::insert_next(Brick*pb0,Brick*pb1)
{
	if(!pb1)	return WRONG_PARAMETER;//wrong parameter
	if(pb0==pb1)	return ARGUMENT_IS_EQUAL;
	pp::Link* pl1=(pp::Link*)((char*)pb1+_offset);
	if(pb0==0)
	{
		if(pl1->pObject!=this)
			this->add_to_begin(pb1);
		else if(_leader.next!=pl1)
			_leader.insert_next(pl1);
	}
	else
	{
		pp::Link* pl0=(pp::Link*)((char*)pb0+_offset);
		if(pl0->pObject!=this)
			return SET_NOT_MATCH;
		if(pl1->pObject!=this)
		{
			if(pl1->pObject)
				((BrickSet*)pl1->pObject)->_size--;
			pl1->pObject=this;
			pl0->insert_next(pl1);
			_size++;
		}
		else
		{
			pl0->insert_next(pl1);
		}
	}
	return 0;
}
void BrickSet::move_forward(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(pl->prev!=&_leader)
		pl->move_forward();

}
void BrickSet::move_backward(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(pl->next!=&_leader)
		pl->move_backward();
}
void	BrickSet::move_to_end_as_new(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(_leader.prev!=pl)
		_leader.insert_prev(pl);
}
void 	BrickSet::move_to_begin(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(_leader.next!=pl)
		_leader.insert_next(pl);
}
void	BrickSet::release(Brick*pb)
{
	pp::Link*pl=(pp::Link*)((char*)pb+_offset);
	if(pl->pObject==this)
	{
		_size--;
		pl->let_alone();
	}
}


}//namespace



