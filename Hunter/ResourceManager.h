#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "stdafx.h"
#include "Resource.h"
#include <map>

//상속받는 클래스에서는 클래스에 맞는 create, destroy 함수를 만들어 주어야 한다

template <typename T>
class ResourceManager
{
public:
	typedef std::map<ResourceHandle, std::string> NameTable;

	ResourceManager();
	virtual ~ResourceManager();

	bool32 Init(uint32 maxSize);
	bool32 ShutDown();

	const T &GetMember(ResourceHandle handle);
	T *GetMemberPointer(ResourceHandle handle);

	bool32 SetName(ResourceHandle handle, const std::string &name);
	const std::string &GetName(ResourceHandle handle);

	uint32 GetMaxSize() { return _maxSize; }
	uint32 GetCurrentSize() { return _currentSize; }

	virtual bool32 Destroy(ResourceHandle handle) = 0;

protected :
	ResourceHandle GetNextFreeHandle() { return _currentSize; }

	T *_datas;
	uint32 _maxSize;
	uint32 _currentSize;
	NameTable _nameTable;
};


template<typename T>
inline ResourceManager<T>::ResourceManager()
	:_datas(nullptr), _maxSize(0), _currentSize(0)
{
	_nameTable.clear();
}

template<typename T>
inline ResourceManager<T>::~ResourceManager()
{
}

template<typename T>
inline bool32 ResourceManager<T>::Init(uint32 maxSize)
{
	_maxSize = maxSize;
	_datas = new T[_maxSize];
	Assert(_datas);
	return true;
}

template<typename T>
inline bool32 ResourceManager<T>::ShutDown()
{
	for (uint32 i = 0; i < _currentSize; ++i)
	{
		if (_datas[i].IsHandleValid())
		{
			_datas[i].Destroy();
		}
	}
	_currentSize = 0;
}

template<typename T>
inline const T & ResourceManager<T>::GetMember(ResourceHandle handle)
{
	Assert(handle < _currentSize)
	return _datas[handle];
}

template<typename T>
inline T * ResourceManager<T>::GetMemberPointer(ResourceHandle handle)
{
	Assert(handle < _currentSize)
	return &_datas[handle];
}

template<typename T>
inline bool32 ResourceManager<T>::SetName(ResourceHandle handle, const std::string &name)
{
	auto find = _nameTable.find(handle);
	if (find != _nameTable.end())
	{
		_nameTable.erase(find);
	}
	else
	{
		_nameTable[handle] = name;
	}
}

template<typename T>
inline const std::string & ResourceManager<T>::GetName(ResourceHandle handle)
{
	auto find = _nameTable.find(handle);
	if (find != _nameTable.end())
	{
		return find->second;
	}
	else
	{
		return std::string();
	}
}

#endif
