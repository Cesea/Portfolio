#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "Typedefs.h"

class LinearAllocator
{
public :
	explicit LinearAllocator(size_t size);
	LinearAllocator(void *start, void *end);

	//지금은 정렬되지 않은 할당을 한다..
	void *Allocate(size_t size, size_t alignment);

	void Free(void *ptr);
	void Reset(void);
private :
	int8 *_start;
	int8 *_end;
	int8 *_current;
};




#endif