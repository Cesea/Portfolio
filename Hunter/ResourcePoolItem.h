#ifndef RESOURCE_POOL_ITEM_H
#define RESOURCE_POOL_ITEM_H

#include "BitFlags.h"
#include "DataPool.h"
#include "ReferenceCounter.h"

class ResourcePoolInterface;

class ResourceCode
{
public:
	enum
	{
		INVALID_CODE = 0xffffffff,
	};

	union
	{
		struct
		{
			uint16		family;
			uint16		type;
		};
		uint32 value;
	};

	ResourceCode() :value(INVALID_CODE) {};
	ResourceCode(const ResourceCode& src) :value(src.value) {};
	ResourceCode(uint32 src) :value(src) {};
	ResourceCode(uint16 family, uint16 type) :family(family), type(type) {};
	~ResourceCode() {};

	ResourceCode& operator=(const ResourceCode& src)
	{
		value = src.value;
		return *this;
	};

	// operater < is provided for sorting purposes
	bool operator<(const ResourceCode& other)
	{
		return value < other.value;
	};

	operator uint32()const { return value; }

	void SetInvalid() { value = INVALID_CODE; }
};


class ResourcePoolItem : public ReferenceCounter
{
public:
	enum State
	{
		Created = 0,	// the resource has been created
		Loaded,	// the resource is filled with data and ready for use
		Disabled,	// the resource is currently disabled for use
		Altered,	// the resource has been altered since loaded
		TotalResourceFlags
	};

	ResourcePoolItem();
	virtual ~ResourcePoolItem();

	virtual bool CreateResource() = 0;
	virtual bool DestroyResource() = 0;
	virtual bool DisableResource() = 0;
	virtual bool RestoreResource() = 0;
	virtual bool LoadResource(const char* filename = nullptr) = 0;
	virtual bool SaveResource(const char* filename = nullptr) = 0;

	// Accessors...
	ResourceCode GetResourceCode()const;
	ResourcePoolInterface* ResourcePool()const;
	PoolHandle ResourceHandle()const;
	Uint32Flags ResourceFlags()const;

	bool IsResourceCreated()const;
	bool IsResourceDisabled()const;
	bool IsResourceLoaded()const;
	const std::string &FindResourceName()const;

	void SetResourceName(const char* name);
	void SetAlteredFlag(bool onOff);
	bool AlteredFlag()const;

	// mimic COM interfaces
	virtual int32 Release();
protected:

	// only derrived classes are permitted to modify internal members
	void SetResourceCode(ResourceCode code);
	void SetResourcePool(ResourcePoolInterface* interfaePtr);
	void SetResourceHandle(PoolHandle handle);
	void SetResourceFlag(int32 flagBit, bool Setting);

	void NotifyCreated();
	void NotifyDisabled();
	void NotifyLoaded();
	void NotifyUnloaded();
	void NotifyRestored();
	void NotifyDestroyed();
	void NotifySaved();

private:

	// Data...
	ResourceCode _resourceCode;
	ResourcePoolInterface* _pResourcePool;
	PoolHandle _resourceHandle;
	Uint32Flags _resourceFlags;

	// Private Functions...

	// Nonexistant Functions...
	ResourcePoolItem(const ResourcePoolItem& Src);
	ResourcePoolItem& operator=(const ResourcePoolItem& Src);

	friend ResourcePoolInterface;
};



inline ResourcePoolItem::ResourcePoolItem()
	:_resourceCode(), _resourceHandle(), _pResourcePool(), _resourceFlags()
{
}

inline ResourcePoolItem::~ResourcePoolItem()
{
	//	debug_assert(!isResourceEnabled(), "this resource has not been disabled");
	//debug_assert(!isResourceCreated(), "this resource has not been destroyed");
}


inline void ResourcePoolItem::SetResourceCode(ResourceCode code)
{
	_resourceCode = code;
}

inline void ResourcePoolItem::SetResourcePool(ResourcePoolInterface* pPoolInterface)
{
	_pResourcePool = pPoolInterface;
}

inline void ResourcePoolItem::SetResourceHandle(PoolHandle handle)
{
	_resourceHandle = handle;
}

inline void ResourcePoolItem::SetResourceFlag(int32 flagBit, bool setting)
{
	_resourceFlags.SetBit(flagBit, setting);
}


inline void ResourcePoolItem::NotifyCreated()
{
	SetResourceFlag(ResourcePoolItem::State::Created, true);
}

inline void ResourcePoolItem::NotifyDestroyed()
{
	SetResourceFlag(ResourcePoolItem::State::Created, false);
}

inline void ResourcePoolItem::NotifyLoaded()
{
	SetAlteredFlag(false);
	SetResourceFlag(ResourcePoolItem::State::Loaded, true);
}

inline void ResourcePoolItem::NotifyUnloaded()
{
	SetResourceFlag(ResourcePoolItem::State::Loaded, false);
}

inline void ResourcePoolItem::NotifyRestored()
{
	SetResourceFlag(ResourcePoolItem::State::Disabled, false);
}

inline void ResourcePoolItem::NotifyDisabled()
{
	SetResourceFlag(ResourcePoolItem::State::Disabled, true);
}

inline void ResourcePoolItem::NotifySaved()
{
	SetAlteredFlag(false);
}

inline ResourcePoolItem & ResourcePoolItem::operator=(const ResourcePoolItem & Src)
{
	// TODO: insert return statement here
}

inline ResourceCode ResourcePoolItem::GetResourceCode() const
{
	return _resourceCode;
}

inline ResourcePoolInterface* ResourcePoolItem::ResourcePool() const
{
	return _pResourcePool;
}

inline PoolHandle ResourcePoolItem::ResourceHandle() const
{
	return _resourceHandle;
}

inline Uint32Flags ResourcePoolItem::ResourceFlags() const
{
	return _resourceFlags;
}

inline bool ResourcePoolItem::IsResourceCreated() const
{
	return _resourceFlags.TestBit(ResourcePoolItem::State::Created);
}

inline bool ResourcePoolItem::IsResourceLoaded() const
{
	return _resourceFlags.TestBit(ResourcePoolItem::State::Loaded);
}


inline bool ResourcePoolItem::IsResourceDisabled() const
{
	return _resourceFlags.TestBit(ResourcePoolItem::State::Disabled);
}



inline void ResourcePoolItem::SetAlteredFlag(bool onOff)
{
	_resourceFlags.SetBit(ResourcePoolItem::State::Altered, onOff);
}

inline bool ResourcePoolItem::AlteredFlag() const
{
	return _resourceFlags.TestBit(ResourcePoolItem::State::Altered);
}




#endif