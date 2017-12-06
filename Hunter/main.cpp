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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE phInstance, LPSTR cmdLine, int cmdShow)
{
	gEngine = new Engine;
	if (!gEngine->InitializePlatform(hInstance))
	{
		return false;
	}

	gEngine->Run();

	SAFE_DELETE(gEngine);

	return 0;
}