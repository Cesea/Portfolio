#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include "DrawCommands.h"

class RenderQueue
{
public :
	static const uint32 queueSize = 1024;

	bool32 Submit(DrawCommand *pCommand);
	DrawCommand *Pop();
	bool32 IsEmpty();

private :

	DrawCommand *_queue[queueSize];

	int32 _head{};
	uint32 _tail{ 0 };
	bool32 _empty{ false };

};

#endif