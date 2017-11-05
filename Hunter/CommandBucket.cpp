#include "stdafx.h"
#include "CommandBucket.h"

CommandBucket::CommandBucket(LinearAllocator * commandPool, uint32 maxCommand)
{
	_maxCommand = maxCommand;
	_keys = new CommandKey[maxCommand];
	_commandHandles = new Handle[maxCommand];
}

CommandBucket::~CommandBucket()
{
	SAFE_DELETE_ARRAY(_keys);
	SAFE_DELETE_ARRAY(_commandHandles);
}

DrawCommand * CommandBucket::GetCommand(Handle handle)
{
	return static_cast<DrawCommand *>(_pCommandIndex->Get(handle));
}

void CommandBucket::Clear()
{
	_pCommandPool->Clear();
	_count = 0;
}

void CommandBucket::Submit()
{
}
