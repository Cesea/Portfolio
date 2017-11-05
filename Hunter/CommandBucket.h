#ifndef COMMAND_BUCKET_H
#define COMMAND_BUCKET_H

#include "PoolIndex.h"
#include "LinearAllocator.h"
#include "PackedArary.h"

#include "DrawCommands.h"

struct CommandKey
{
	int32 key;
};

class CommandBucket
{
public :
	CommandBucket(LinearAllocator *commandPool, uint32 maxCommand);
	~CommandBucket();

	template <typename CommandDataType>
	Handle CreateCommand(CommandKey key);

	template <typename CommandDataType>
	CommandDataType *GetCommandData(Handle handle);

	DrawCommand *GetCommand(Handle handle);

	void Clear();

	void Submit();

private :
	int32 _count{0};
	uint32 _maxCommand;
	CommandKey *_keys;
	Handle *_commandHandles;

	PoolIndex *_pCommandIndex;
	LinearAllocator *_pCommandPool;

};


template<typename CommandDataType>
inline Handle CommandBucket::CreateCommand(CommandKey key)
{
	DrawCommand *pCommand = new (_pCommandPool) DrawCommand;

	CommandDataType *dataType = new (_pCommandPool) CommandDataType;

	pCommand->dispatch = CommandData<CommandDataType>::Dispatchfunction;
	Handle commandHandle = _pCommandIndex->CreateHandle((void *)pCommand);

	_keys[_count] = key;
	_commandHandles[_count] = commandHandle;
	_count++;

	return commandHandle;
}

template<typename CommandDataType>
inline CommandDataType * CommandBucket::GetCommandData(Handle handle)
{
	return NULL;
}

#endif