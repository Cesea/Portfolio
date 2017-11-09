#include "stdafx.h"
#include "Allocators.h"

LinearAllocator::LinearAllocator(size_t size)
{
	_start = (int8 *)malloc(size);
	_current = _start;
	_end = _start + size;
}

LinearAllocator::LinearAllocator(void * start, void * end)
{
	_start = (int8 *)start;
	_end = (int8 *)_end;
	_current = _start;
}

void * LinearAllocator::Allocate(size_t size, size_t alignment)
{
	_current += size;
	void *result = _current;
	if (_end < _current)
	{
		Assert(false); //메모리 할당에 실패하였다
		return nullptr;
	}
	return result;
}

void LinearAllocator::Free(void * ptr)
{
// 아무것도 하지 않는다
}

void LinearAllocator::Reset(void)
{
	_current = _start;
}
