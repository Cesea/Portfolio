#include "stdafx.h"
#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(uint32 size)
{
	_pStart = new char[size];
	_pEnd = _pStart + size;
	_pCurrent = _pStart;
}

LinearAllocator::~LinearAllocator()
{
	delete[] _pStart;
}

void * LinearAllocator::Allocate(uint32 size)
{
	if (size == 0 || _pCurrent + size > _pEnd)
	{
		return nullptr;
	}

	void *pBlock = _pCurrent;
	_pCurrent += size;
	return pBlock;
}

uint32 LinearAllocator::MaxSize()
{
	return _pEnd - _pStart;
}

uint32 LinearAllocator::CurrentSize()
{
	return _pCurrent - _pStart;
}

void LinearAllocator::Clear()
{
	_pCurrent = _pStart;
}

void * operator new(size_t size, LinearAllocator & allocator)
{
	return allocator.Allocate(size);
}

void operator delete(void * ptr, LinearAllocator & allocator)
{
	// do nothing.  Linear allocators do not free individual allocations.  Only entire resets of the pool matter.
}
