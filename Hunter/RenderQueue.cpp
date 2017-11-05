#include "stdafx.h"
#include "RenderQueue.h"

bool32 RenderQueue::Submit(DrawCommand * pCommand)
{
	if (!_empty &&_head == _tail)
	{
		return false;
	}
	_queue[_tail] = pCommand;
	_tail = _tail + 1 % queueSize;
	_empty = false;
	return true;
}

DrawCommand * RenderQueue::Pop()
{
	if (_empty || _head == _tail)
	{
		return nullptr;
	}

	DrawCommand *pCommand = _queue[_head];
	_head = _head + 1 % queueSize;
	if (_head == _tail)
	{
		_empty = true;
	}
	return pCommand;
}

bool32 RenderQueue::IsEmpty()
{
	return _empty;
}
