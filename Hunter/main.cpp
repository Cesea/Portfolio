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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE phInstance, LPSTR cmdLine, int cmdShow)
{
	MemoryArena<LinearAllocator, SingleThreadPolicy> arena(1024 * 100);

	//int32 *a = (int32 *)HUNTER_NEW(int32, arena);
	//*a = 1;

	//int32 *b = (int32 *)HUNTER_NEW(int32, arena);
	//*b = 2;

	//int32 *c = (int32 *)HUNTER_NEW(int32, arena);
	//*c = 3;

	//int32 *d = (int32 *)HUNTER_NEW(int32, arena);

	//*d = *a * *c;

	//for (int i = 0; i < 20000; ++i)
	//{
	//	int32 *d = (int32 *)HUNTER_NEW(int32, arena);
	//	HUNTER_DELETE(d, arena);
	//}

	for (int i = 0; i < 20000; ++i)
	{
		int32 *d = new int32;
		delete d;
	}


	arena.Reset();


	gEngine = new Engine;
	if (!gEngine->InitializePlatform(hInstance))
	{
		return false;
	}

	gEngine->Run();

	SAFE_DELETE(gEngine);

	return 0;
}