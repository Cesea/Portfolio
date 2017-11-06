#ifndef RESOURCE_POOL_H
#define RESOURCE_POOL_H

#include "ResourcePoolItem.h"
#include "ResourcePoolManager.h"

class ResourcePoolInterface
{
public:
	union FILE_EXTENSION
	{
		char ext[4];
		uint32 fourcc;
	};

	typedef std::map<std::string, PoolHandle> NameTable;

	ResourcePoolInterface();
	virtual ~ResourcePoolInterface() {};

	// these functions must be proviided in the derived class (cResourceType)
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized()const = 0;

	virtual void DestroyAll() = 0;
	virtual void DisableAll() = 0;
	virtual void RestoreAll() = 0;
	virtual void Clean() = 0;

	ResourcePoolItem* CreateResource(const std::string& resourceName);
	ResourcePoolItem* LoadResource(const std::string& filename);
	ResourcePoolItem* GetResource(PoolHandle handle);
	ResourcePoolItem* FindResource(const std::string& name);
	void DestroyResource(ResourcePoolItem* pResource);

	bool SaveResource(ResourcePoolItem* pResource);

	PoolHandle FindResourceHandle(const std::string& name);
	const std::string* FindResourceName(PoolHandle handle) const;
	void SetResourceName(PoolHandle handle, const char* name);

	void RegisterResourcePool(ResourceCode code);
	void UnregisterResourcePool();

	ResourceCode RegistrationCode() const;

protected:
	ResourceCode _registrationCode;
	NameTable _nameTable;

private:
	// Private Functions...
	virtual PoolHandle InternalCreateResource(const std::string& resourceName) = 0;
	virtual void InternalDestroyResource(PoolHandle handle) = 0;
	virtual ResourcePoolItem* InternalGetResource(PoolHandle handle) = 0;
};


#endif