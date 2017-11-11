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

HandleAlloc::HandleAlloc(uint16 maxHandles)
	: _numHandles(0)
	, _maxHandles(maxHandles)
{
	Reset();
}

HandleAlloc::~HandleAlloc()
{
}

const uint16* HandleAlloc::GetHandles() const
{
	return GetDensePtr();
}

uint16 HandleAlloc::GetHandleAt(uint16 at) const
{
	return GetDensePtr()[at];
}

uint16 HandleAlloc::GetNumHandles() const
{
	return _numHandles;
}

uint16 HandleAlloc::GetMaxHandles() const
{
	return _maxHandles;
}

uint16 HandleAlloc::Alloc()
{
	if (_numHandles < _maxHandles)
	{
		uint16_t index = _numHandles;
		++_numHandles;

		uint16* dense = GetDensePtr();
		uint16  handle = dense[index];
		uint16* sparse = GetSparsePtr();
		sparse[handle] = index;
		return handle;
	}

	return invalidIndex;
}

bool HandleAlloc::IsValid(uint16 handle) const
{
	uint16 *dense = GetDensePtr();
	uint16 *sparse = GetSparsePtr();
	uint16  index = sparse[handle];

	return (index < _numHandles) && (dense[index] == handle);
}

void HandleAlloc::Free(uint16 handle)
{
	uint16* dense = GetDensePtr();
	uint16* sparse = GetSparsePtr();
	uint16 index = sparse[handle];
	--_numHandles;
	uint16 temp = dense[_numHandles];
	dense[_numHandles] = handle;
	sparse[temp] = index;
	dense[index] = temp;
}

void HandleAlloc::Reset()
{
	_numHandles = 0;
	uint16* dense = GetDensePtr();
	for (uint16_t i = 0, num = _maxHandles; i < num; ++i)
	{
		dense[i] = i;
	}
}

uint16* HandleAlloc::GetDensePtr() const
{
	uint8* ptr = (uint8 *)reinterpret_cast<const uint8*>(this);
	return (uint16*)&ptr[sizeof(HandleAlloc)];
}

uint16* HandleAlloc::GetSparsePtr() const
{
	return &GetDensePtr()[_maxHandles];
}

//HandleAlloc* CreateHandleAlloc(AllocatorI* _allocator, uint16_t _maxHandles)
//{
//	uint8_t* ptr = (uint8_t*)BX_ALLOC(_allocator, sizeof(HandleAlloc) + 2 * _maxHandles * sizeof(uint16_t));
//	return BX_PLACEMENT_NEW(ptr, HandleAlloc)(_maxHandles);
//}
//
//void DestroyHandleAlloc(AllocatorI* _allocator, HandleAlloc* _handleAlloc)
//{
//	_handleAlloc->~HandleAlloc();
//	BX_FREE(_allocator, _handleAlloc);
//}