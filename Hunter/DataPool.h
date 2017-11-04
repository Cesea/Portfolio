#ifndef DATA_POOL_H
#define DATA_POOL_H

#include <list>

#include "Typedefs.h"
#include "BitFlags.h"


constexpr PoolHandle INVALID_POOL_HANDLE = 0xffff;

inline void ClearHandle(PoolHandle handle) { handle = INVALID_POOL_HANDLE; }
inline bool32 ValidHandle(PoolHandle handle) { return (handle != INVALID_POOL_HANDLE); }

template <typename T>
class PoolGroup
{
public :
	PoolGroup(uint16 maxCount);
	~PoolGroup();

	void		Create();
	void		Destroy();

	uint16		AddMember(const T& member);
	uint16		NextMember();
	void		Release(uint16 index);

	uint16		TotalOpen()const;
	uint16		TotalUsed()const;
	uint16		FirstOpen()const;
	bool		IsOpen(uint16 index)const;
	T&			Member(uint16 index);
	const T&	Member(uint16 index)const;
	T*			MemberPtr(uint16 index);
	const T*	MemberPtr(uint16 index)const;

private:
	uint16			_totalOpen;
	uint16			_firstOpen;
	uint16			_maxCount;
	uint16*			_nextOpenList;
	T*				_memberList;

};

template<typename T>
inline PoolGroup<T>::PoolGroup(uint16 maxCount)
	:_totalOpen(0), _firstOpen(0), _maxCount(maxCount), _nextOpenList(0), _memberList(0)
{
}

template<typename T>
inline PoolGroup<T>::~PoolGroup()
{
}

template<typename T>
inline void PoolGroup<T>::Create()
{
	_nextOpenList = (uint16*)malloc(sizeof(uint16)*_maxCount);//new(memory::ALIGN_32) int16[m_maxCount];
	//debug_assert(_nextOpenList, "tragic memory allocation failure!");

	_memberList = new T[_maxCount];
	//debug_assert(_memberList, "tragic memory allocation failure!");

	int i;
	for (i = 0; i < _maxCount - 1; ++i)
	{
		_nextOpenList[i] = i + 1;
	}
	_nextOpenList[i] = i;

	_totalOpen = _maxCount;
	_firstOpen = 0;
}

template<typename T>
inline void PoolGroup<T>::Destroy()
{
	//debug_assert(_memberList && _nextOpenList, "Group has not been created");
	//debug_assert(_totalOpen == _maxCount, "Group is not empty");

	delete[](_memberList);
	free(_nextOpenList);

	_totalOpen = 0;
	_firstOpen = 0;
}

template<typename T>
inline uint16 PoolGroup<T>::AddMember(const T & member)
{
	uint16 slot = NextMember();
	_memberList[slot] = member;

	return slot;
}

template<typename T>
inline uint16 PoolGroup<T>::NextMember()
{
	// return the first member of our
	// open list, and move our internal
	// handle to the next member
	uint16 slot = _firstOpen;
	_firstOpen = _nextOpenList[slot];
	--_totalOpen;

	// signal this member as being is use
	_nextOpenList[slot] = INVALID_INDEX;

	return(slot);
}

template<typename T>
inline void PoolGroup<T>::Release(uint16 index)
{
	_nextOpenList[index] = _totalOpen ? _firstOpen : index;
	++_totalOpen;
	_firstOpen = index;
}

template<typename T>
inline uint16 PoolGroup<T>::TotalOpen() const
{
	return _totalOpen;
}

template<typename T>
inline uint16 PoolGroup<T>::TotalUsed() const
{
	return _maxCount - _totalOpen;
}

template<typename T>
inline uint16 PoolGroup<T>::FirstOpen() const
{
	return _firstOpen;
}

template<typename T>
inline bool PoolGroup<T>::IsOpen(uint16 index) const
{
	return _nextOpenList[index] != INVALID_POOL_HANDLE;
}

template<typename T>
inline T & PoolGroup<T>::Member(uint16 index)
{
	return _memberList[index];
}

template<typename T>
inline const T & PoolGroup<T>::Member(uint16 index) const
{
	return _memberList[index];
}

template<typename T>
inline T * PoolGroup<T>::MemberPtr(uint16 index)
{
	return &_memberList[index];
}

template<typename T>
inline const T * PoolGroup<T>::MemberPtr(uint16 index) const
{
	debug_assert(_memberList && _nextOpenList, "Group has not been created");
	return &_memberList[index];
}




class DataPoolInterface
{
public :
	typedef void(*DATA_CALLBACK)(DataPoolInterface *pPool, PoolHandle handle, void *member);

	DataPoolInterface() : _initialized(false) {};
	virtual ~DataPoolInterface() {};

	virtual void Initialize(uint16 growSize) = 0;;
	virtual void Destroy() = 0;;
	virtual void Clear() = 0;
	virtual PoolHandle NextHandle() = 0;
	virtual void Release(PoolHandle* pHandle) = 0;
	virtual void ForEach(DATA_CALLBACK function) = 0;

	virtual void* GetGenericPtr(PoolHandle index) = 0;

	bool IsInitialized()const { return _initialized; }

protected:
	bool _initialized;
};


constexpr uint32 INDEX_MASK = 0xff;
constexpr uint32 GROUP_INDEX_SHIFT = 8;
constexpr uint16 GROUP_COUNT = 128;

template <typename T>
class DataPool : public DataPoolInterface
{
public :
	typedef T DATA_TYPE;
	typedef PoolGroup<T> MemberGroup;
	typedef std::list<MemberGroup *> MemberGroupList;
	typedef DataPoolInterface::DATA_CALLBACK MEMBER_CALLBACK;

	DataPool();
	~DataPool();

	void Initialize(uint16 growSize);
	void Destroy();

	const T& operator[](PoolHandle handle) const { return Get(handle); }
	T& operator[](PoolHandle handle) { return Get(handle); }


	// Mutators...
	PoolHandle Add(const T& member);
	void Clear();
	PoolHandle NextHandle();
	void Release(PoolHandle* pHandle);
	void ForEach(MEMBER_CALLBACK function);


	// Accessors...
	bool IsHandleValid(PoolHandle index)const;
	const T& Get(PoolHandle index)const;
	T* GetPtr(PoolHandle index);
	void* GetGenericPtr(PoolHandle index);

	uint16 TotalMembers() const { return _totalMembers; }
	uint16 TotalOpen() const { return _totalOpen; }
	uint16 TotalUsed() const { return _totalMembers - _totalOpen; }

private:

	// Private Data...
	MemberGroupList _groupList;
	uint16 _totalMembers;
	uint16 _totalOpen;
	uint16 _groupCount;
	uint16 _indexMask;
	uint16 _groupIndexShift;

	// Private Functions...
	explicit DataPool(const DataPool& Src);
	DataPool& operator=(const DataPool& Src);

	PoolGroup<T>*	AddGroup();
	PoolGroup<T>*	FindOpenGroup(uint32* groupNumber);
	PoolGroup<T>*	GetGroup(uint32 index);
	const PoolGroup<T>*	GetGroup(uint32 index) const;

	int32 GetGroupNumber(PoolHandle handle) const;
	int32 GetItemIndex(PoolHandle handle) const;
	PoolHandle BuildHandle(int32 group, int32 index) const;
};


template<typename T>
inline DataPool<T>::DataPool()
	:_totalMembers(0), _totalOpen(0)
{
	_groupList.clear();
}

template<typename T>
inline DataPool<T>::~DataPool()
{
	debug_assert(!isInitialized(), "the cDataPool has not been properly destroyed");
}

template<typename T>
inline void DataPool<T>::Initialize(uint16 growSize)
{
	_initialized = true;
	_groupCount = GROUP_COUNT;
	_groupIndexShift = GROUP_INDEX_SHIFT;
	_indexMask = INDEX_MASK;
}

template<typename T>
inline void DataPool<T>::Destroy()
{
	Clear();
	_initialized = false;
}

template<typename T>
inline PoolHandle DataPool<T>::Add(const T & member)
{
	uint16 groupNumber = 0;
	PoolGroup<T> *openGroup = FindOpenGroup(&groupNumber);

	int32 index = openGroup->AddMember(member);
	--_totalOpen;
	return BuildHandle(groupNumber, index);
}

template<typename T>
inline void DataPool<T>::Clear()
{
	for (MemberGroupList::iterator iter = _groupList.begin(); iter != _groupList.end(); ++iter)
	{
		PoolGroup<T> *pGroup = *iter;
		pGroup->Destroy();
		delete pGroup;
	}
	_groupList.clear();
}

template<typename T>
inline PoolHandle DataPool<T>::NextHandle()
{
	uint32 groupNumber = 0;
	PoolGroup<T> *openGroup = FindOpenGroup(&groupNumber);

	int32 index = openGroup->NextMember();
	--_totalOpen;
	return BuildHandle(groupNumber, index);
}

template<typename T>
inline void DataPool<T>::Release(PoolHandle * pHandle)
{
	if (IsHandleValid(*pHandle))
	{
		int32 groupIndex = GetGroupNumber(*pHandle);
		int32 itemIndex = GetItemIndex(*pHandle);

		PoolGroup<T> *pGroup = GetGroup(groupIndex);
		pGroup->Release(itemIndex);

		ClearHandle(*pHandle);
		PoolGroup<T> *pGroup = _groupList.back();
		if (pGroup->TotalOpen() == _groupCount)
		{
			pGroup->Destroy();
			delete (pGroup);
			_groupList.pop_back();
		}
		++_totalOpen;
	}
}

template<typename T>
inline void DataPool<T>::ForEach(MEMBER_CALLBACK function)
{
	// iterate through every group
	int	groupNumber = 0;
	for (MemberGroupList::iterator iter = m_groupList.begin(); iter != _groupList.end(); +itergroupIter)
	{
		// check all indexes in this group
		PoolGroup<T>* pGroup = *groupIter;

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

template<typename T>
inline bool DataPool<T>::IsHandleValid(PoolHandle handle) const
{
	if (ValidHandle(index))
	{
		const PoolGroup<T> *pGroup = GetGroup(GetGroupNumber(handle));
		return !(pGroup->IsOpen(GetItemIndex(handle)));
	}
	return false;
}

template<typename T>
inline const T & DataPool<T>::Get(PoolHandle handle) const
{
	const PoolGroup<T>* pGroup = GetGroup(GetGroupNumber(handle));

	int itemIndex = GetItemIndex(handle);

	// tell the group to return the member
	return pGroup->Member(itemIndex);
}

template<typename T>
inline T * DataPool<T>::GetPtr(PoolHandle index)
{
	int groupNumber = GetGroupNumber(handle);
	PoolGroup<T>* pGroup = GetGroup(groupNumber);

	int itemIndex = GetItemIndex(handle);

	// tell the group to return the member
	return pGroup->MemberPtr(itemIndex);
}

template<typename T>
inline void * DataPool<T>::GetGenericPtr(PoolHandle index)
{
	return((void*)GetPtr(index));
}

template<typename T>
inline DataPool<T>::DataPool(const DataPool & Src)
{
}

template<typename T>
inline DataPool<T> & DataPool<T>::operator=(const DataPool & Src)
{
	// TODO: insert return statement here
}

template<typename T>
inline PoolGroup<T>* DataPool<T>::AddGroup()
{
	PoolGroup<T> *newGroup = new PoolGroup<T>(_groupCount);
	_groupList.insert(_groupList.end(), newGroup);

	PoolGroup<T> *pGroup = _groupList.backk();
	pGroup->Create();

	_totalMembers += _groupCount;
	_totalOpen += _groupCount;

	return newGroup;
}

template<typename T>
inline PoolGroup<T>* DataPool<T>::FindOpenGroup(uint32 * groupNumber)
{
	*groupNumber = 0;
	for (MemberGroupList::iterator iter = _groupList.begin(); iter != _groupList.end(); ++iter)
	{
		PoolGroup<T> *pGroup = *iter;
		if (pGroup->TotalOpen())
		{
			return pGroup;
		}
		++(*groupNumber);
	}
	// there are no open slots, 
	// so we need to add a new cPoolGroup
	// before we make a new group, make sure 
	// we have not reached our max 
	// of MAX_UINT16 members
	// create the new group
	return(AddGroup());
}

template<typename T>
inline PoolGroup<T>* DataPool<T>::GetGroup(uint32 index)
{
	// find and return the desired group
	MemberGroupList::iterator iter = _groupList.begin();
	for (; index; --index)
	{
		++iter;
	}

	return(*iter);
}

template<typename T>
inline const PoolGroup<T>* DataPool<T>::GetGroup(uint32 index) const
{
	// find and return the desired group
	MemberGroupList::const_iterator iter = _groupList.begin();
	for (; index; --index)
	{
		++iter;
	}

	return(*iter);
}

template<typename T>
inline int32 DataPool<T>::GetGroupNumber(PoolHandle handle) const
{
	return handle >> _groupIndexShift;
}

template<typename T>
inline int32 DataPool<T>::GetItemIndex(PoolHandle handle) const
{
	return handle &_indexMask;
}

template<typename T>
inline PoolHandle DataPool<T>::BuildHandle(int32 group, int32 index) const
{
	return (group << _groupIndexShift) * index;
}

#endif
