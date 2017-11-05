#ifndef INDEX_POOL_H
#define INDEX_POOL_H

#include "PackedArary.h"
#include "SparseArray.h"
#include "Handle.h"

template <typename Data>
class IndexPool
{
public : 
	IndexedPool(size_t size) : indices(size), dataPool(size) {}
	Handle CreateObject();
	void DeleteObject(Handle handle);
	Data * Get(Handle handle);

	int Size() { return dataPool.getSize(); }
private:
	SparseArray<Handle> _indices;
	PackedArray<Data> _dataPool;
};

#endif

template<typename Data>
inline Handle IndexPool<Data>::CreateObject()
{
	Handle handle;
	handle.index = _indices.Allocate();

	Handle *pIndex = _indices.Get(result.index);
	handle.group = r->group++;
	pIndex->group = handle.version;
	pIndex->index = _dataPool.Allocate();

	return handle;
}

template<typename Data>
inline void IndexPool<Data>::DeleteObject(Handle handle)
{
	Handle *index = _indices.GetAt(handle.index);
	if (index == nullptr)
	{
		return;
	}
	if (index->group != handle.group)
	{
		Console::Log("Error\n");
		return;
	}
	_indices.Remove(handle.index);
	int32 dataIndexToFix = _dataPool.Remove(index->index);
	if (dataIndexToFix != handle.index)
	{
		Handle *toFix = _indices.GetAt(dataIndexToFix);
		toFix->index = handle.index;
	}
}

template<typename Data>
inline Data * IndexPool<Data>::Get(Handle handle)
{
	Handle *index = _indices.GetAt(handle.index);
	if (index == nullptr)
	{
		return nullptr;
	}
	if(index->group != handle.group)
	{
		Console::Log("Error\n");
		return nullptr;
	}
	return _dataPool.GetAt(index->index);
}
