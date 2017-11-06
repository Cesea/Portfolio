#ifndef RESOURCE_POOL_ITEM_H
#define RESOURCE_POOL_ITEM_H

#include "ReferenceCounter.h"
#include "DataPool.h"

class ResourcePoolInterface;

class ResourceCode
{
public :
	enum
	{
		INVALID_CODE = 0xffffffff
	};
	union 
	{
		uint32 value;
		struct
		{
			uint16 family;
			uint16 type;
		};
	};

	ResourceCode();
	ResourceCode(const ResourceCode &other);
	ResourceCode(uint32 source);
	ResourceCode(uint16 family, uint16 type);
	~ResourceCode();

	bool operator<(const ResourceCode &other)
	{
		return (this->value) < other.value;
	}

	void SetInvalid() { value = INVALID_CODE; }
};

inline bool	operator< (const ResourceCode &r1, const ResourceCode &r2)
{
	return r1.value < r2.value;
}

class ResourcePoolItem : public ReferenceCounter
{
public :
	enum ResourceState
	{
		Created = 0,
		Loaded,
		Disabled,
		Altered,
		NumStateFlags
	};

	ResourcePoolItem();
	virtual ~ResourcePoolItem();

	virtual bool32 CreateResource() = 0;
	virtual bool32 DestroyResource() = 0;
	virtual bool32 DisableResource() = 0;
	virtual bool32 RestoreResource() = 0;
	virtual bool32 LoadResource(const std::string &fileName) = 0;
	virtual bool32 SaveResource(const std::string &fileName) = 0;

	ResourceCode GetResourceCode() const;
	const ResourcePoolInterface *GetResourcePool() const;
	PoolHandle GetResourceHandle() const;
	Uint32Flags GetResourceFlags() const;

	bool32 IsResourceCreated() const;
	bool32 IsResourceDisabled() const;
	bool32 IsResourceLoaded() const;
	const std::string &FindResourceName() const;

	void SetResourceName(const std::string &name);
	void SetAlteredFlag(bool b);
	bool32 IsAlteredFlag() const;

	virtual int32 Release();

protected :
	void SetResourceCode(const ResourceCode code);
	void SetResourcePool(ResourcePoolInterface *pPool);
	void SetResourceHandle(PoolHandle handle);
	void SetResourceFlags(int32 flagbit, bool setting);

	void NotifyCreated();
	void NotifyDisabled();
	void NotifyLoaded();
	void NotifyUnloaded();
	void NotifyRestore();
	void NotifyDestroyed();
	void NotifySaved();

private :
	ResourceCode _resourceCode;
	ResourcePoolInterface *_pResourcePool;
	PoolHandle _resourceHandle;
	Uint32Flags _resourceFlags;

	ResourcePoolItem(const ResourcePoolItem &other);
	ResourcePoolItem &operator= (const ResourcePoolItem &other);

	friend ResourcePoolInterface;
};


#endif