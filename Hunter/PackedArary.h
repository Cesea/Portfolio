#ifndef PACKED_ARRAY_H
#define PACKED_ARRAY_H

#include "Typedefs.h"

template <typename T>
class PackedArray
{
public :
	PackedArray(uint32 size);

	int32 Allocate();
	int32 Remove(int32 index);
	T *GetAt(int32 index);

	int32 Size() { return _end; }

	void Clear();

private :
	T *_dataArrays;
	int32 _end{ 0 };
	uint32 _size;
};


template<typename T>
inline PackedArray<T>::PackedArray(uint32 size)
	:_size(size)
{
	_dataArrays = new T[size];
}

template<typename T>
inline int32 PackedArray<T>::Allocate()
{
	//Array is full
	if (_end == _size)
	{
		return -1;
	}
	T *pData = new (&_dataArrays[_end]) T;
	return _end++;
}

template<typename T>
inline int32 PackedArray<T>::Remove(int32 index)
{
	_end--;
	if (index != _end)
	{
		_dataArrays[index] = _dataArrays[_end];
		return _end;
	}
	return -1;
}

template<typename T>
inline T * PackedArray<T>::GetAt(int32 index)
{
	if (index > _size)
	{
		return nullptr;
	}
	return &_dataArrays[index];
}

template<typename T>
inline void PackedArray<T>::Clear()
{
	_end = 0;
	_size = 0;
}


#endif
