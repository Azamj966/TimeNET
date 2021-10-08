#ifndef EVENTLIST_H
#define EVENTLIST_H
#include <set>
#include "gmnet.h"
#include "DateTime.h"

class Binding;

//dereferencing less: dereference pointers, then compare contents 
template
<
typename Key_T
>
struct DerefLess
{
	bool operator()(
		const Key_T& _Left, 
		const Key_T& _Right
		) const
	{
		return *_Left<*_Right;
	}
};

typedef std::set< Binding*, DerefLess<Binding*>, GCALLOC(Binding*) > EventList;

#endif
