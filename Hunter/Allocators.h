#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "Typedefs.h"

class LinearAllocator
{
public :
	explicit LinearAllocator(size_t size);
	LinearAllocator(void *start, void *end);

	//������ ���ĵ��� ���� �Ҵ��� �Ѵ�..
	void *Allocate(size_t size, size_t alignment);

	void Free(void *ptr);
	void Reset(void);
private :
	int8 *_start;
	int8 *_end;
	int8 *_current;
};




#endif