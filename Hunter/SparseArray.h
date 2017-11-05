#ifndef SPARSE_ARRAY_H
#define SPARSE_ARRAY_H

#include "Typedefs.h"

template <typename T>
struct DataElement
{
	T data;
	int32 next;
};

template <typename T>
class SparseArray
{
public :
	SparseArray(uint32 size)
		:_size(size)
	{
		_dataArray = new DataElement<T>[size]; InitGroup();
	}

	int32 Allocate();

	void Remove(int32 index);
	T *GetAt(int32 index);

private :

	void InitGroup();

	DataElement<T> *_dataArray;

	bool _hasFree{ false };
	int32 _free;
	int32 _end{0};
	uint32 _size;
};


template<typename T>
inline int32 SparseArray<T>::Allocate()
{
	if (_end == _size)
	{
		return -1;
	}

	if (!_hasFree)
	{
		return _end++;
	}
	else
	{
		int32 dataLoc = _free;
		int32 next = _dataArray[_free].next;
		if (next == -1)
		{
			_hasFree = false;
		}
		else
		{
			_free = next;
		}
		_size++;
		return dataLoc;
	}
}

template<typename T>
inline void SparseArray<T>::Remove(int32 index)
{
	if (_size == 0 || index >= end)
	{
		return;
	}

	int oldFree = free;
	free = index;
	_dataArray[index].next = oldFree;
	_size--;
}

template<typename T>
inline T * SparseArray<T>::GetAt(int32 index)
{
	return (index < _size) ? _dataArray[index] : nullptr;
}

template<typename T>
inline void SparseArray<T>::InitGroup()
{
	for (int32 i = 0; i < _size; ++i)
	{
		_dataArray[i].data.group = 0;
	}
}

#endif
