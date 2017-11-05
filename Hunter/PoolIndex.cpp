#include "stdafx.h"
#include "PoolIndex.h"

PoolIndex::PoolIndex(uint32 size)
	:_indices(size)
{
}

Handle PoolIndex::CreateHandle()
{
	Handle handle;
	handle.index = _indices.Allocate();
	InnerHandle *index = _indices.GetAt(handle.index);
	handle.group = index->group++;
	index->group = handle.group;
	return handle;
}

Handle PoolIndex::CreateHandle(void * pData)
{
	Handle handle = CreateHandle();
	SetInnerIndexValue(handle, pData);
	return handle;
}

bool PoolIndex::IsValid(Handle handle)
{
	InnerHandle *index = _indices.GetAt(handle.index);
	if (handle.group == index->group)
	{
		return true;
	}
	return false;
}

void PoolIndex::SetInnerIndexValue(Handle handle, void *pData)
{
	if (IsValid(handle))
	{
		_indices.GetAt(handle.index)->pData = pData;
	}
}

void * PoolIndex::Get(Handle handle)
{
	return IsValid(handle) ? _indices.GetAt(handle.index)->pData : nullptr;
}
