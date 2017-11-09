#include "stdafx.h"
#include <iostream>

DEFINE_META_POD(int8);
DEFINE_META_POD(int16);
DEFINE_META_POD(int32);
DEFINE_META_POD(int64);

DEFINE_META_POD(uint8);
DEFINE_META_POD(uint16);
DEFINE_META_POD(uint32);
DEFINE_META_POD(uint64);

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


//template <typename T, typename Arena>
//void Delete(T *object, Arena &arena)
//{
//	object->~T();
//	arena.Free(object);
//}
//
//template <typename T, typename Arena>
//T *NewArray(Arena &arena, size_t N, const char *file, int32 line)
//{
//	union
//	{
//		void *as_void;
//		size_t *as_size_t;
//		T *as_T;
//	};
//
//	as_void = arena.Allocate(sizeof(T) * N + sizeof(size_t), file, line);
//	*as_size_t++ = N;
//
//	//여기에 N이 더해지는것이 맞는지 아닌지를 확인하자
//	const T *const onePastLast = as_T + N;
//	while (as_T < onePastLast)
//	{
//		new (as_T++) T;
//	}
//
//	return (as_t - N);
//}
//
//template <typename T, typename Arena>
//void DeleteArray(T *ptr, Arena &arena)
//{
//	union
//	{
//		size_t *as_size_t;
//		T *as_T;
//	};
//
//	as_T = ptr;
//	const size_t N = as_size_t[-1];
//
//	for (size_t i = N; i > 0; --i)
//	{
//		as_T[i - 1].~T();
//	}
//	arena.Free(as_size_t - 1);
//}


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