#include "stdafx.h"
#include "Console.h"

char Console::buffer[] = {};

Console::Console()
{
}

Console::~Console()
{
	Shutdown();
}

bool32 Console::Init()
{
	bool32 result = true;
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
		_allocated = true;
	}
	else
	{
		result = false;
	}
	return result;
}

void Console::Shutdown()
{
	if (_allocated)
	{
		FreeConsole();
	}
}

void Console::Log(char *str, ...)
{
	ZeroMemory(buffer, sizeof(buffer));
	va_list ap;

	va_start(ap, str);
	vsprintf(buffer + strlen(buffer), str, ap);
	va_end(ap);

	printf(buffer);
}


