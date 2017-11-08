#include "stdafx.h"
#include <iostream>

DEFINE_META_POD(int8);
DEFINE_META_POD(int16);
DEFINE_META_POD(int32);

DEFINE_META_POD(uint8);
DEFINE_META_POD(uint16);
DEFINE_META_POD(uint32);

struct foo
{
	foo(float a, float b, float c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}
	float a;
	float b;
	float c;
};

void *operator new(size_t bytes, const char *file, int32 line)
{
	void *result = malloc(bytes);
	OutputDebugString(file);
	return result;
}

template <class T> 
struct TypeAndCount
{
};

template <class T, size_t N>
struct TypeAndCount<T[N]>
{
	typedef T Type;
	static const size_t Count = N;
};

//template <typename Allocator>
//void *operator new(size_t bytes, Allocator &allocator, const char *file, int32 line)
//{
//	void *result = allocator.Allocate(bytes);
//	return result;
//}
//
//template <typename Allocator>
//void operator delete(void *ptr, Allocator &allocator, const char *file, int32 line)
//{
//	allocator.Free(ptr);
//}

template <typename T, typename Arena>
void Delete(T *object, Arena &arena)
{
	object->~T();
	arena.Free(object);
}

template <typename T, typename Arena>
T *NewArray(Arena &arena, size_t n, const char *file, int32 line)
{
	union
	{
		void *as_void;
		size_t *as_size_t;
		T *as_T;
	};

	as_void = arena.Allocate(sizeof(T) * n + sizeof(size_t), file, line);
	*as_size_t++ = n;

	const T *const onePastLast = as_T + n;
	while (as_T < onePastLast)
	{
		new (as_T++) T;
	}

	return (as_t - n);
}

template <typename T, typename Arena>
void DeleteArray(T *ptr, Arena &arena)
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
	arena.Free(as_size_t - 1);
}

#define HUNTER_NEW(type, arena) new (arena.Allocate(sizeof(type), __FILE__, __LINE__)) type
#define HUNTER_DELETE(type, arena) Delete(type, arena)
#define HUNTER_NEW_ARRAY(type, arena) NewArray<TypeAndCount<type>::Type>(arena, TypeAndCount<type>::Count, __FILE__, __LINE__)
#define HUNTER_DELETE_ARRAY(type, arena) DeleteArray(type, arena);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE phInstance, LPSTR cmdLine, int cmdShow)
{

	/*Test *test = HUNTER_NEW(Test, arena)(0, 1, 2);
	HUNTER_DELETE(test, arena);*/


	gEngine = new Engine;
	if (!gEngine->InitializePlatform(hInstance))
	{
		return false;
	}

	gEngine->Run();

	SAFE_DELETE(gEngine);

	return 0;
}