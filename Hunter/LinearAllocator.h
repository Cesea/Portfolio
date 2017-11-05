#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "Typedefs.h"

class LinearAllocator
{
public :
	explicit LinearAllocator(uint32 size);
	~LinearAllocator();

	void *Allocate(uint32 size);
	void Clear();

private :
	char *_pStart;
	char *_pEnd;
	char *_pCurrent;
};

void *operator new(size_t size, LinearAllocator &allocator);
void operator delete(void *ptr, LinearAllocator &allocator);

#endif