#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include <stdio.h>

class Console
{
public:
	Console();
	virtual ~Console();

	bool32 Init();
	void Shutdown();

	static void Log(char *str, ...);
private:

	static char buffer[256];

	bool _allocated{ false };
};


#endif