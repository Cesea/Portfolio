#ifndef DATA_POOL_H
#define DATA_POOL_H

#include "Typedefs.h"
#include <list>

typedef uint16 PoolHandle;

constexpr uint16 INVALID_HANDLE = 0xffff;

inline void ClearHandle(PoolHandle handle) { handle = INVALID_HANDLE; }
inline bool32 HandleValid(PoolHandle handle) { return handle != INVALID_HANDLE; }

template <typename T>
class PoolGroup
{
public :
	PoolGroup(uint16 maxCount);
	~PoolGroup(){}
	
	void		Create();
	void		Destroy();

	uint16		AddMember(const T& member);
	uint16		NextMember();
	void		Release(uint16 index);

	uint16		TotalOpen() const;
	uint16		TotalUsed() const;
	uint16		FirstOpen() const;
	bool		IsOpen(uint16 index) const;
	T&			Member(uint16 index);
	const T&	Member(uint16 index) const;
	T*			MemberPointer(uint16 index);
	const T*	MemberPointer(uint16 index) const;

private:
	uint16 _totalOpen;
	uint16 _firstOpen;
	uint16 _maxCount;
	uint16 *_nextOpenList;
	T *_memberList;
};

template<typename T>
inline PoolGroup<T>::PoolGroup(uint16 maxCount)
	:_maxCount(maxCount), 
	_totalOpen(0), 
	_firstOpen(0), 
	_nextOpenList(nullptr), 
	_memberList(nullptr)
{
}

template <class T>
inline void PoolGroup<T>::Create()
{
	_nextOpenList = new uint16[_maxCount];
	Assert(_nextOpenList) //Memory Allocation failed

	_memberList = new T[_maxCount];
	Assert(_memberList) //Memory Allocation failed

	for (int32 i = 0; i < _maxCount - 1; ++i)
	{
		_nextOpenList[i] = i + 1;
	}
	_nextOpenList[_maxCount - 1] = _maxCount - 1;

	_totalOpen = _maxCount;
	_firstOpen = 0;
}

template <class T>
inline void PoolGroup<T>::Destroy()
{
	SAFE_DELETE_ARRAY(_memberList);
	SAFE_DELETE_ARRAY(_nextOpenList);

	_totalOpen = 0;
	_firstOpen = 0;
}


template <class T>
inline uint16 PoolGroup<T>::NextMember()
{
	Assert(_totalOpen > 0); //no open list
	uint16 slot = _firstOpen;
	_firstOpen = _nextOpenList[slot];
	--_totalOpen;

	_nextOpenList[slot] = INVALID_HANDLE;

	return(slot);
}

template <class T>
inline uint16 PoolGroup<T>::AddMember(const T& member)
{
	uint16 slot = NextMember();
	_memberList[slot] = member;

	return(slot);
}

template <class T>
inline void PoolGroup<T>::Release(uint16 index)
{
	Assert(index < _maxCount);
	Assert(!IsOpen(index));

	_nextOpenList[index] = _totalOpen ? _firstOpen : index;
	++_totalOpen;
	_firstOpen = index;
}


template <class T>
inline uint16 PoolGroup<T>::TotalOpen() const
{
	return _totalOpen;
}

template <class T>
inline uint16 PoolGroup<T>::TotalUsed() const
{
	return _maxCount - _totalOpen;
}

template <class T>
inline uint16 PoolGroup<T>::FirstOpen() const
{
	return _firstOpen;
}

template <class T>
inline bool PoolGroup<T>::IsOpen(uint16 index) const
{
	Assert(index < _maxCount);
	return _nextOpenList[index] != INVALID_HANDLE;
}

template <class T>
inline T& PoolGroup<T>::Member(uint16 index)
{
	Assert(index < _firstOpen); //Accessing invalid index
	return _memberList[index];
}

template <class T>
inline const T& PoolGroup<T>::Member(uint16 index) const
{
	Assert(index < _firstOpen); //Accessing invalid index
	return _memberList[index];
}

template <class T>
inline T* PoolGroup<T>::MemberPointer(uint16 index)
{
	Assert(index < _firstOpen); //Accessing invalid index
	return &_memberList[index];
}

template <class T>
inline const T* PoolGroup<T>::MemberPointer(uint16 index) const
{
	Assert(index < _firstOpen); //Accessing invalid index
	return &_memberList[index];
}

//DataPoolInterfaace /////////////////////////////////////////////////////////////


class DataPoolInterface
{
public :
	typedef void(*MEMBER_CALLBACK)(DataPoolInterface *pPool, PoolHandle handle, void *member);
	DataPoolInterface() {}
	virtual ~DataPoolInterface() {}

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Clear() = 0;
	virtual PoolHandle NextHandle() = 0;
	virtual void Release(PoolHandle *pHandle) = 0;
	virtual void ForEach(MEMBER_CALLBACK function) = 0;

	virtual void *GetGenericPointer(PoolHandle handle) = 0;

	bool32 IsInitialized() const { return _initialized; }

protected :
	bool32 _initialized;
};

// DataPool //////////////////////////////////////////////////////////////

	constexpr uint32 GROUP_COUNT = 0x100;
	constexpr uint32 INDEX_SHIFT = 8;
	constexpr uint32 INDEX_MASK = GROUP_COUNT - 1;

template <class DataType>
class DataPool : public DataPoolInterface
{
public :

	typedef PoolGroup<DataType> MemberGroup;
	typedef std::list<MemberGroup *> MemberGroupList;
	typedef DataPoolInterface::MEMBER_CALLBACK MEMBER_CALLBACK;

	DataPool();
	virtual ~DataPool();

	void Initialize() override;
	void Destroy() override;

	const DataType &operator[] (PoolHandle handle) const { return Get(handle); }
	DataType &operator[] (PoolHandle handle) { return Get(handle); }

	PoolHandle Add(const DataType &member);
	void Clear();
	PoolHandle NextHandle();
	void Release(PoolHandle *pHandle);
	void ForEach(MEMBER_CALLBACK function) override;

	bool32 IsHandleValid(PoolHandle index) const;
	const DataType &Get(PoolHandle index) const;
	const DataType *GetPointer(PoolHandle index) const;
	void *GetGenericPointer(PoolHandle handle) override;

	uint16 TotalMember() const { return _totalMembers; }
	uint16 TotalOpen() const { return _totalOpen; }
	uint16 TotalUsed() const { return _totalmembers - totalCount; }

private:
	MemberGroupList _groupList;
	uint16 _totalMembers;
	uint16 _totalOpen;
	uint16 _groupCount;
	uint32 _indexMask;
	int32 _indexShift;
	// Private Functions...
	explicit DataPool(const DataPool& Src);
	DataPool& operator=(const DataPool& Src);

	PoolGroup<DataType>*	AddGroup();
	PoolGroup<DataType>*	FindOpenGroup(uint32* groupNumber);
	PoolGroup<DataType>*	GetGroup(uint32 index);
	const PoolGroup<DataType>*	GetGroup(uint32 index) const;

	int32 GetGroupNumber(PoolHandle handle) const;
	int32 GetItemIndex(PoolHandle handle) const;
	PoolHandle BuildHandle(int32 group, int32 index) const;
};



template<class DataType>
inline DataPool<DataType>::DataPool()
	:_totalMembers(0), _totalOpen(0)
{
	_groupList.clear();
}

template<class DataType>
inline DataPool<DataType>::~DataPool()
{
	Assert(!IsInitialized()); //destroy를 호출 하고 나서 하기...
}

template<class DataType>
inline void DataPool<DataType>::Initialize()
{
	Assert(!IsInitialized());
	_initialized = true;

	_groupCount = GROUP_COUNT;
	_indexShift = INDEX_SHIFT;
	_indexMask = INDEX_MASK;

}

template<class DataType>
inline void DataPool<DataType>::Destroy()
{
	Clear();
	_initialized = false;
}

template<class DataType>
inline PoolHandle DataPool<DataType>::Add(const DataType & member)
{
	uint32 groupNumber = 0;
	PoolGroup<DataType> *openGroup = FindOpenGroup(&groupNumber);
	int32 index = openGroup->AddMember(member);
	--_totalOpen;
	return BuildHandle(groupNumber, index);
}

template<class DataType>
inline void DataPool<DataType>::Clear()
{
	// destroy all groups in the list
	for (MemberGroupList::iterator iter = _groupList.begin();
		iter != _groupList.end(); ++iter)
	{
		PoolGroup<DataType>* pGroup = *iter;
		pGroup->Destroy();
		delete pGroup;
	}

	// now clear the list itself
	_groupList.clear();
}

template<class DataType>
inline PoolHandle DataPool<DataType>::NextHandle()
{
	uint32 groupNumber = 0;
	PoolGroup<DataType> *openGroup = FindOpenGroup(&groupNumber);
	int32 index = openGroup->NextMember();
	--_totalOpen;
	return BuildHandle(groupNumber, index);
}

template<class DataType>
inline void DataPool<DataType>::Release(PoolHandle * pHandle)
{
	if (IsHandleValid(*pHandle))
	{
		int32 groupIndex = GetGroupNumber(*pHandle);
		int32 itemIndex = GetItemIndex(*pHandle);

		PoolGroup<DataType> *group = GetGroup(groupIndex);
		group->Release(itemIndex);

		ClearHandle(*pHandle);
		//마지막 그룹을 없앨 수 있는지 확인한다.
		PoolGroup<DataType> *lastGroup = _groupList.back();
		if (lastGroup->TotalOpen() == _groupCount)
		{
			lastGroup->Destroy();
			delete(lastGroup);
			_groupList.pop_back();

			//TODO : 다 없에는게 좋을까????
			_totalMembers -= _groupCount;
		}
		++_totalOpen;
	}
}

template<class DataType>
inline void DataPool<DataType>::ForEach(MEMBER_CALLBACK function)
{
	int	groupNumber = 0;
	for (MemberGroupList::iterator iter = _groupList.begin();
		iter != _groupList.end(); ++iter)
	{
		// check all indexes in this group
		PoolGroup<DataType>* pGroup = *iter;

		int callbackCount = pGroup->TotalUsed();
		uint16	itemIndex = 0;
		while (callbackCount && itemIndex < _groupCount)
		{
			// is the member in use?
			if (!pGroup->IsOpen(itemIndex))
			{
				// issue the callback
				function(this, BuildHandle(groupNumber, itemIndex), (void*)&(pGroup->Member(itemIndex)));
				--callbackCount;
			}
			++itemIndex;
		}
		++groupNumber;
	}
}

template<class DataType>
inline bool32 DataPool<DataType>::IsHandleValid(PoolHandle handle) const
{
	if (HandleValid(handle))
	{
		const PoolGroup<DataType> *group = GetGroup(GetGroupNumber(handle));
		return (!group->IsOpen(GetItemIndex(handle)));
	}
	return false;
}

template<class DataType>
inline const DataType & DataPool<DataType>::Get(PoolHandle handle) const
{
	const PoolGroup<DataType> *group = GetGroup(GetGroupNumber(handle));
	int32 itemIndex = GetItemIndex(handle);
	return group->Member(itemIndex);
}

template<class DataType>
inline const DataType * DataPool<DataType>::GetPointer(PoolHandle handle) const
{
	const PoolGroup<DataType> *group = GetGroup(GetGroupNumber(handle));
	int32 itemIndex = GetItemIndex(handle);
	return group->MemberPointer(itemIndex);
}

template<class DataType>
inline void * DataPool<DataType>::GetGenericPointer(PoolHandle handle)
{
	return (void *)GetPointer(handle);
}

template<class DataType>
inline DataPool<DataType>::DataPool(const DataPool & Src)
{
	return;
}

template<class DataType>
inline DataPool<DataType> & DataPool<DataType>::operator=(const DataPool & Src)
{
	// TODO: insert return statement here
	return DataPool<DataType>();
}

template<class DataType>
inline PoolGroup<DataType>* DataPool<DataType>::AddGroup()
{
	PoolGroup<DataType> *pNewGroup = new PoolGroup<DataType>(_groupCount);
	_groupList.insert(_groupList.end(), pNewGroup);

	PoolGroup<DataType> *pGroup = _groupList.back();
	pGroup->Create();

	_totalOpen += _groupCount;
	_totalMembers += _groupCount;
	return pGroup;
}

template<class DataType>
inline PoolGroup<DataType>* DataPool<DataType>::FindOpenGroup(uint32 * groupNumber)
{
	*groupNumber = 0;
	for (MemberGroupList::iterator iter = _groupList.begin(); iter != _groupList.end(); ++iter)
	{
		PoolGroup<DataType> *pGroup = *iter;
		if (pGroup->TotalOpen())
		{
			return pGroup;
		}
		++(*groupNumber);
	}
	return (AddGroup());
}

template<class DataType>
inline PoolGroup<DataType>* DataPool<DataType>::GetGroup(uint32 index)
{
	MemberGroupList::iterator iter = _groupList.begin();
	for (; index; --index)
	{
		iter++;
	}
	return (*iter);
}

template<class DataType>
inline const PoolGroup<DataType>* DataPool<DataType>::GetGroup(uint32 index) const
{
	MemberGroupList::const_iterator iter = _groupList.begin();
	for (; index; --index)
	{
		++iter;
	}

	return(*iter);
}

template<class DataType>
inline int32 DataPool<DataType>::GetGroupNumber(PoolHandle handle) const
{
	return handle >> _indexShift;
}

template<class DataType>
inline int32 DataPool<DataType>::GetItemIndex(PoolHandle handle) const
{
	return handle & _indexMask;
}

template<class DataType>
inline PoolHandle DataPool<DataType>::BuildHandle(int32 group, int32 index) const
{
	return (group << _indexShift) + index;
}


#endif
