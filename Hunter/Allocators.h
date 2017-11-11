#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "Typedefs.h"

inline bool IsAligned(const void* ptr, size_t align)
{
	union { const void* ptr; uintptr_t addr; } un;
	un.ptr = ptr;
	return 0 == (un.addr & (align - 1));
}


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

#define invalidIndex (0xffff)

///
class HandleAlloc
{
public:
	///
	HandleAlloc(uint16 maxHandles);

	///
	~HandleAlloc();

	const uint16_t* GetHandles() const;

	uint16 GetHandleAt(uint16 _at) const;

	uint16 GetNumHandles() const;

	uint16 GetMaxHandles() const;

	uint16 Alloc();

	bool IsValid(uint16 handle) const;

	void Free(uint16 handle);

	void Reset();

private:
	HandleAlloc();

	uint16* GetDensePtr() const;

	uint16* GetSparsePtr() const;

	uint16 _numHandles;
	uint16 _maxHandles;
};

//HandleAlloc* CreateHandleAlloc(AllocatorI* _allocator, uint16_t _maxHandles);
//void DestroyHandleAlloc(AllocatorI* _allocator, HandleAlloc* _handleAlloc);



#endif