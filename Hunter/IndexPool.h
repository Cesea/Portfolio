#ifndef INDEX_POOL_H
#define INDEX_POOL_H

#include "PackedArary.h"
#include "SparseArray.h"
#include "Handle.h"

template <typename Data>
class IndexedPool
{
public : 
	IndexedPool(uint32 size) : _maxSize(size), indices(size), dataPool(size) {}
	Handle CreateObject();
	void DeleteObject(Handle handle);
	Data * Get(Handle handle);

	int Size() { return dataPool.getSize(); }
private:
	SparseArray<Handle> _indices;
	PackedArray<Data> _dataPool;

	uint32 _baseVersion{ 0 };
	uint32 _maxSize;
};

#endif

template<typename Data>
inline Handle IndexedPool<Data>::CreateObject()
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
inline void IndexedPool<Data>::DeleteObject(Handle handle)
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
inline Data * IndexedPool<Data>::Get(Handle handle)
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
