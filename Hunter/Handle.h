#ifndef HANDLE_H
#define HANDLE_H

#include "Typedefs.h"

constexpr uint16 INVALID_INDEX = 0xffff;


struct Handle
{
	Handle()
		:index(INVALID_INDEX)
	{}

	Handle(uint16 val)
		:index(val)
	{}

	uint16 index;

	bool32 IsValid()
	{
		return (index != INVALID_INDEX);
	}

	void MakeUnVaild()
	{
		index = INVALID_INDEX;
	}
};

static Handle INVALID_HANDLE;

typedef Handle VertexBufferHandle;
typedef Handle IndexBufferHandle;
typedef Handle EffectHandle;
typedef Handle TextureHandle;
typedef Handle SurfaceMaterialHandle;
typedef Handle VertexDeclHandle;

#endif