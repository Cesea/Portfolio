#ifndef COMMAND_PACKET_H
#define COMMAND_PACKET_H

#include "BackendDispatch.h"

typedef void *CommandPacket;

namespace commandpacket
{
	static const size_t OFFSET_NEXT_COMMAND_PACKET = 0u;
	static const size_t OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(CommandPacket);
	static const size_t OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(BackendDispatchFunction);

	template <typename T>
	inline CommandPacket Create(size_t auxMemorySize)
	{
		return ::operator new(GetSize<T>(auxMemorySize));
	}

	template <typename T>
	inline size_t GetSize(size_t auxMemorySize)
	{
		return OFFSET_COMMAND + sizeof(T) + auxMemorySize;
	}

	inline CommandPacket *GetNextCommandPacket(CommandPacket packet)
	{
		return reinterpret_cast<CommandPacket *>(reinterpret_cast<char *>(packet) + OFFSET_NEXT_COMMAND_PACKET);
	}

	template <typename T>
	inline CommandPacket* GetNextCommandPacket(T* command)
	{
		return reinterpret_cast<CommandPacket*>(reinterpret_cast<char*>(command) - OFFSET_COMMAND + OFFSET_NEXT_COMMAND_PACKET);
	}

	inline BackendDispatchFunction* GetBackendDispatchFunction(CommandPacket packet)
	{
		return reinterpret_cast<BackendDispatchFunction*>(reinterpret_cast<char*>(packet) + OFFSET_BACKEND_DISPATCH_FUNCTION);
	}

	template <typename T>
	inline T *GetCommand(CommandPacket packet)
	{
		return reinterpret_cast<T *>(reinterpret_cast<char *>(packet) + OFFSET_COMMAND);
	}

	template <typename T>
	inline char *GetAuxiliaryMemory(T *command)
	{
		return reinterpret_cast<char *>(command + sizeof(T));
	}

	inline void StoreNextCommandPacket(CommandPacket packet, CommandPacket nextPacket)
	{
		*commandpacket::GetNextCommandPacket(packet) = nextPacket;
	}

	template <typename T>
	inline void StoreNextCommandPacket(T *command, CommandPacket nextPacket)
	{
		*commandpacket::GetBackendDispatchFunction<T>(command) = nextPacket;
	}

	inline void StoreBackendDispatchFunction(CommandPacket packet, BackendDispatchFunction function)
	{
		*commandpacket::GetBackendDispatchFunction(packet) = function;
	}

	inline const CommandPacket LoadNextCommandPacket(const CommandPacket packet)
	{
		return *GetNextCommandPacket(packet);
	}

	inline const BackendDispatchFunction LoadBackendDispatchFunction(const CommandPacket packet)
	{
		return *GetBackendDispatchFunction(packet);
	}

	inline const void *LoadCommand(const CommandPacket packet)
	{
		return reinterpret_cast<char *>(packet) + OFFSET_COMMAND;
	}

}

#endif