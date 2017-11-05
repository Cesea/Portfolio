#ifndef SPARSE_ARRAY_H
#define SPARSE_ARRAY_H

#include "Typedefs.h"

template <typename T>
struct DataElement
{
	T data;
	int32 next{-1};
};

template <typename T>
class SparseArray
{
public :
	SparseArray(uint32 maxSize)
		:_maxSize(maxSize)
	{
		_dataArray = new DataElement<T>[maxSize]; 
	}

	int32 Allocate();

	void Remove(int32 index);
	T *GetAt(int32 index);
	uint32 NumElements() { return _numElement; }
	void Clear();

private :

	static const int32 NO_MORE_FREE = -1;

	DataElement<T> *_dataArray;

	bool _hasFree{ false };
	int32 _free{NO_MORE_FREE};
	int32 _end{0};
	uint32 _maxSize;

	uint32 _numElement{0};
};


template<typename T>
inline int32 SparseArray<T>::Allocate()
{
	if (_end == _maxSize && _numElement == _maxSize)
	{
		return -1;
	}

	int dataLocation;
	if (_free == NO_MORE_FREE)
	{
		dataLocation = _end++;
	}
	else
	{
		dataLocation = _free;
		_free = _dataArray[_free].next;
	}

	_numElement++;
	return dataLocation;

}

template<typename T>
inline void SparseArray<T>::Remove(int32 index)
{
	if (_size == 0 || index >= end)
	{
		return;
	}
	if (index == _maxSize - 1)
	{
		_end--;
	}
	else
	{
		int32 oldFree = _free;
		_free = index;
		_dataArray[index].next = oldFree;
	}
	_numElement--;
}

template<typename T>
inline T * SparseArray<T>::GetAt(int32 index)
{
	return (index < _maxSize) ? (&_dataArray[index].data) : (nullptr);
}

template<typename T>
inline void SparseArray<T>::Clear()
{
	_free = NO_MORE_FREE;
	_numElement = 0;
	_end = 0;
}


#endif
