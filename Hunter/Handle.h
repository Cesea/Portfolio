#ifndef HANDLE_H
#define HANDLE_H

#include "Typedefs.h"

struct Handle
{
	uint32 index{ 0 };
	uint16 group{ 0 };

	void ResetGroup() { group = 0; }
};

#endif