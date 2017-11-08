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

template <Allocator>
void *operator new(size_t bytes, Allocator &allocator, const char *file, lin32 line)
{
	void *result = nullptr;
	return result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE phInstance, LPSTR cmdLine, int cmdShow)
{
	
	foo *a = new(__FILE__, __LINE__) foo(12.0f, 4.0f, 5.0f);

	void *pData = malloc(1000);

	//foo *f = new (__FILE__, __LINE__) foo(1.0f, 2.0f, 3.0f);



	gEngine = new Engine;
	if (!gEngine->InitializePlatform(hInstance))
	{
		return false;
	}

	gEngine->Run();

	SAFE_DELETE(gEngine);

	return 0;
}