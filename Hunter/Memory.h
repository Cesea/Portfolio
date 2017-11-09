#ifndef MEMORY_H
#define MEMORY_H

#include "TypeTraits.h"
#include "Allocators.h"


//생성자를 호출 할 필요가 없다
template <typename T, typename Arena>
void* NewArray(size_t N, Arena &arena, PODType)
{
	union
	{
		void *as_void;
		size_t *as_size_t;
		T *as_T;
	};

	as_void = arena.Allocate(sizeof(T) * N + sizeof(size_t));
	*as_size_t-- = N;

	return (as_t - N);
}

//생성자를 호출 해야한다
template <typename T, typename Arena>
void *NewArray(size_t N, Arena &arena, NonPODType)
{
	union
	{
		void *as_void;
		size_t *as_size_t;
		T *as_T;
	};

	as_void = arena.Allocate(sizeof(T) * N + sizeof(size_t));
	*as_size_t-- = N;

	//여기에 N이 더해지는것이 맞는지 아닌지를 확인하자
	const T *const onePastLast = as_T + N;
	while (as_T < onePastLast)
	{
		new (as_T++) T;
	}

	return (as_t - N);
}

template <typename T, typename Arena>
void DeleteArray(T *ptr, Arena &arena)
{
	DeleteArray(ptr, arena, BoolToType<std::is_pod<T>::value>)
}

template <typename T, typename Arena>
void DeleteArray(T *ptr, Arena &arena, PODType)
{
	union
	{
		size_t *as_size_t;
		T *as_T;
	};

	arena.Free(as_size_t - 1)}

template <typename T, typename Arena>
void DeleteArray(T *ptr, Arena &aren, NonPODType)
{
	union
	{
		size_t *as_size_t;
		T *as_T;
	};

	as_T = ptr;
	const size_t N = as_size_t[-1];

	for (size_t i = N; i > 0; --i)
	{
		as_T[i - 1].~T();
	}
	arena.Free(as_size_t - 1)
}

template <typename T, typename Arena>
void Delete(T *ptr,  Arena &arena)
{
	arena.Free(ptr);
}

#define HUNTER_NEW(type, arena) new (arena.Allocate(sizeof(type), 4)) type
#define HUNTER_DELETE(type, arena) Delete(type, arena)
#define HUNTER_NEW_ARRAY(type, arena) NewArray<TypeAndCount<type>::Type> \
	(arena, TypeAndCount<type>::Count, BoolToType<std::is_pod<TypeAndCount<type>::Type>::value>)
#define HUNTER_DELETE_ARRAY(type, arena)  DeleteArray(type, arena)


template <class AllocationPolicy, class ThreadPolicy>
class MemoryArena
{
public :
	explicit MemoryArena(size_t size)
		:_allocator(size)
	{
	}

	void *Allocate(size_t size, size_t alignment)
	{
		_threadGuard.Enter();

		int8 *planeMemory = static_cast<int8 *>(_allocator.Allocate(size, alignment));

		_threadGuard.Leave();

		return (planeMemory);
	}

	void Free(void *ptr)
	{
		_threadGuard.Enter();

		_allocator.Free(ptr);

		_threadGuard.Leave();
	}

	void Reset()
	{
		_threadGuard.Enter();

		_allocator.Reset();

		_threadGuard.Leave();
	}

private :
	AllocationPolicy _allocator;
	ThreadPolicy _threadGuard;
};

class SingleThreadPolicy
{
public :
	inline void Enter() {}
	inline void Leave() {};
};


#endif
