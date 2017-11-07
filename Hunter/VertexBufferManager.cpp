#include "stdafx.h"
#include "VertexBufferManager.h"

VertexBufferManager::VertexBufferManager()
{
}

VertexBufferManager::~VertexBufferManager()
{
}

VertexBufferHandle VertexBufferManager::CreateStatic(uint32 numVertex, uint32 stride, const void * pData)
{
	ResourceHandle handle = GetNextFreeHandle();
	if (_datas[handle].Create(numVertex, stride, false, pData))
	{
		_datas[handle].SetHandle(handle);
		_currentSize++;
		return handle;
	}
	return INVALID_HANDLE;
}

VertexBufferHandle VertexBufferManager::CreateDynamic(uint32 numVertex, uint32 stride)
{
		_currentSize++;
	return VertexBufferHandle();
}

bool32 VertexBufferManager::Destroy(ResourceHandle handle)
{
	return _datas[handle].Destroy();
}

