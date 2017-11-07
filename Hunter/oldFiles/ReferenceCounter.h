#ifndef REFERENCE_COUNTER_H
#define REFERENCE_COUNTER_H

#include "Typedefs.h"

class ReferenceCounter
{
public :
	ReferenceCounter();
	ReferenceCounter(const ReferenceCounter &other);
	ReferenceCounter &operator= (const ReferenceCounter &other);
	virtual ~ReferenceCounter();


	int32 AddRef();
	int32 Release();

	int32 GetCount();

private :
	int32 _count;

};

#endif