#ifndef COMMAND_BUCKET_H
#define COMMAND_BUCKET_H

#include "CommandPacket.h"

#include "RenderContext.h"

template <typename Key>
class CommandBucket
{
public :
	CommandBucket();

	bool32 Init(uint32 maxSize, const Matrix &view, const Matrix &projection);

	template <typename U>
	U *AddCommand(Key key, size_t auxMemorySize);

	template <typename U, typename V>
	U *AppendCommand(V *command, size_t auxMemorySize);

	void Sort();
	void Submit();

	void Clear();

private :
	void SubmitPacket(const CommandPacket packet);

	void DecodeKey(Key key);

private :
	uint32 _maxSize;
	uint32 _currentSize;
	Key *_keys;
	CommandPacket *_packets;

	Matrix _viewMatrix;
	Matrix _projectionMatrix;

	RenderContext _context;
};


template<typename Key>
inline CommandBucket<Key>::CommandBucket()
{
	
}

template<typename Key>
inline bool32 CommandBucket<Key>::Init(uint32 maxSize, const Matrix & view, const Matrix & projection)
{
	_currentSize = 0;
	_maxSize = maxSize;
	_viewMatrix = view;
	_projectionMatrix = projection;
	_keys = new Key[_maxSize];
	_packets = new void *[_maxSize];
	return true;
}

template<typename Key>
inline void CommandBucket<Key>::Sort()
{
}

template<typename Key>
inline void CommandBucket<Key>::Submit()
{
	gpDevice->SetTransform(D3DTS_VIEW, &_viewMatrix);
	gpDevice->SetTransform(D3DTS_PROJECTION, &_projectionMatrix);

	for (uint32 i = 0; i < _currentSize; ++i)
	{
		CommandPacket packet = _packets[i];
		do
		{
			SubmitPacket(packet);
			packet = commandpacket::LoadNextCommandPacket(packet);
		} while (packet != nullptr);
	}
}

template<typename Key>
inline void CommandBucket<Key>::Clear()
{
	for (int i = 0; i < _currentSize; ++i)
	{
		SAFE_DELETE(_packets[i]);
	}
	_currentSize = 0;
}

template<typename Key>
inline void CommandBucket<Key>::SubmitPacket(const CommandPacket packet)
{
	const BackendDispatchFunction function = commandpacket::LoadBackendDispatchFunction(packet);
	const void *command = commandpacket::LoadCommand(packet);
	function(command);
}

template<typename Key>
inline void CommandBucket<Key>::DecodeKey(Key key)
{
}

template<typename Key>
template<typename U>
inline U * CommandBucket<Key>::AddCommand(Key key, size_t auxMemorySize)
{
	CommandPacket packet = commandpacket::Create<U>(auxMemorySize);
	//store key and pointer to the data
	{
		const uint32 current = _currentSize++;
		_keys[current] = key;
		_packets[current] = packet;
	}

	commandpacket::StoreNextCommandPacket(packet, nullptr);
	commandpacket::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

	return commandpacket::GetCommand<U>(packet);
}

template<typename Key>
template<typename U, typename V>
inline U * CommandBucket<Key>::AppendCommand(V * command, size_t auxMemorySize)
{
	CommandPacket packet = commandpacket::Create<U>(auxMemorySize);

	commandpacket::StoreNextCommandPacket<V>(command, packet);

	commandpacket::StoreNextCommandPacket(packet, nullptr);
	commandpacket::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

	return commandpacket::GetCommand<U>(packet);
}


#endif
