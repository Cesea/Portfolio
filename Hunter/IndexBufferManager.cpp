#include "stdafx.h"
#include "IndexBufferManager.h"

IndexBufferManager::IndexBufferManager()
{
	_currentSize = 0;
}

IndexBufferManager::~IndexBufferManager()
{
}

IndexBufferHandle IndexBufferManager::CreateStatic(uint32 numVertex, IndexBuffer::IndexFormat format, const void * pData)
{
	IndexBufferHandle handle = GetNextFreeHandle();
	if (_datas[handle].Create(numVertex, format, false, pData))
	{
		_datas[handle].SetHandle(handle);
		_currentSize++;
		return handle;
	}
	return INVALID_HANDLE;
}

IndexBufferHandle IndexBufferManager::CreateDynamic(uint32 numVertex, uint32 stride)
{
	_currentSize++;
	return VertexBufferHandle();
}

bool32 IndexBufferManager::Destroy(IndexBufferHandle handle)
{
	return _datas[handle].Destroy();
}
