#ifndef GAME_OBJECT_HANDLE_H
#define GAME_OBJECT_HANDLE_H

#include "VideoResource.h"

struct RockHandle : public ResourceHandle
{
	RockHandle()
	{
		this->index = 0;
		this->count = 0;
	}
	RockHandle(uint16 index, uint16 count)
	{
		this->index = index;
		this->count = count;
	}
};

struct TreeHandle : public ResourceHandle
{
	TreeHandle()
	{
		this->index = 0;
		this->count = 0;
	}
	TreeHandle(uint16 index, uint16 count)
	{
		this->index = index;
		this->count = count;
	}
};

struct SnakeHandle : public ResourceHandle
{
	SnakeHandle()
	{
		this->index = 0;
		this->count = 0;
	}
	SnakeHandle(uint16 index, uint16 count)
	{
		this->index = index;
		this->count = count;
	}
};

#endif
