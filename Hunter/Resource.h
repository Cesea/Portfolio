#ifndef RESOURCE_H
#define RESOURCE_H

#include "Typedefs.h"

constexpr uint16 INVALID_HANDLE = 0xffff;

inline void ClearHandle(ResourceHandle handle) { handle = INVALID_HANDLE; }
inline bool32 HandleValid(ResourceHandle handle) { return handle != INVALID_HANDLE; }

class Resource
{
public:
	Resource() { _handle = INVALID_HANDLE; }
	virtual ~Resource() {}

	virtual ResourceHandle Create() { return INVALID_HANDLE; }
	virtual bool32 Destroy() { return false; }

	ResourceHandle GetHandle() { return _handle; }
	void SetHandle(ResourceHandle handle) { _handle = handle; }
	bool32 IsHandleValid() { return HandleValid(_handle); }

protected:
	ResourceHandle _handle;
};

#endif
