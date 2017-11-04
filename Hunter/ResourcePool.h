#ifndef RESOURCE_POOL_H
#define RESOURCE_POOL_H

#if defined  FindResource
#undef FindResource
#endif

#include "DataPool.h"
#include "ResourcePoolItem.h"
#include "ResourcePoolManager.h"

#include <map>

class ResourcePoolInterface
{
public :

	union FILE_EXTENSION
	{
		char ext[4];
		uint32 fourcc;
	};
	typedef std::map<std::string, PoolHandle> ResourceNameMap;

	ResourcePoolInterface()
		:_registrationCode(ResourceCode::INVALID_CODE)
	{
		_nameMap.clear();
	};

	virtual ~ResourcePoolInterface() {};

	// these functions must be proviided in the derived class (cResourceType)
	virtual void Initialize(uint16 growSize) = 0;
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
	const std::string &FindResourceName(PoolHandle handle)const;
	void SetResourceName(PoolHandle handle, const char* name);

	void RegisterResourcePool(ResourceCode code);
	void UnregisterResourcePool();

	ResourceCode RegistrationCode() const;

protected:
	ResourceCode _registrationCode;
	ResourceNameMap _nameMap;

private:
	// Private Functions...
	virtual PoolHandle InternalCreateResource(const std::string &resourceName) = 0;
	virtual void InternalDestroyResource(PoolHandle handle) = 0;
	virtual ResourcePoolItem* InternalGetResource(PoolHandle handle) = 0;
};

inline void ResourcePoolInterface::RegisterResourcePool(ResourceCode code)
{
	_registrationCode = code;
	RESOURCEPOOLS->RegisterResourcePool(_registrationCode, this);
}

inline void ResourcePoolInterface::UnregisterResourcePool()
{
	RESOURCEPOOLS->UnregisterResourcePool(_registrationCode);
	_registrationCode.SetInvalid();
}

inline ResourceCode ResourcePoolInterface::RegistrationCode() const
{
	return _registrationCode;
}


inline PoolHandle ResourcePoolInterface::FindResourceHandle(const std::string &name)
{
	PoolHandle newHandle(0);

	ClearHandle(newHandle);

	// look up the name in our map
	ResourceNameMap::const_iterator iter = _nameMap.find(name);
	if (iter != _nameMap.end())
	{
		newHandle = iter->second;
	}

	return newHandle;
}


inline const std::string &ResourcePoolInterface::FindResourceName(PoolHandle handle) const
{
	ResourceNameMap::const_iterator iter = _nameMap.begin();
	for (; iter != _nameMap.end(); ++iter)
	{
		if (handle == iter->second)
		{
			return iter->first;
		}
	}
	return std::string();
}

inline void ResourcePoolInterface::SetResourceName(PoolHandle handle, const char* name)
{
	ResourceNameMap::iterator iter = _nameMap.begin();
	for (; iter != _nameMap.end(); ++iter)
	{
		if (handle == iter->second)
		{
			iter = _nameMap.erase(iter);
			break;
		}
	}

	_nameMap[std::string(name)] = handle;
}

inline ResourcePoolItem* ResourcePoolInterface::CreateResource(const std::string &resourceName)
{
	PoolHandle handle = InternalCreateResource(resourceName);

	if (ValidHandle(handle))
	{
		ResourcePoolItem* pResource = GetResource(handle);

		pResource->SetResourcePool(this);
		pResource->SetResourceHandle(handle);
		pResource->SetResourceCode(_registrationCode);
		return pResource;
	}

	return 0;
}

inline ResourcePoolItem* ResourcePoolInterface::LoadResource(const std::string &resourceName)
{
	// does the resource already exist?
	ResourcePoolItem* pResource = FindResource(resourceName);

	if (!pResource)
	{
		// create a new resource
		pResource = CreateResource(resourceName);

		if (pResource)
		{
			// attempt to load the desired data
			if (pResource->LoadResource())
			{
				// ok!
				return pResource;
			}

			// loading failed.
			// destroy the resource we created
			//destroyResource(pResource);
			pResource->Release();
			pResource = 0;
		}
	}

	return pResource;
}

inline bool ResourcePoolInterface::SaveResource(ResourcePoolItem* pResource)
{
	if (pResource)
	{
		// save the resource using it's own name as the file path
		return pResource->SaveResource(0);
	}
	return false;
}

inline ResourcePoolItem* ResourcePoolInterface::FindResource(const std::string &name)
{
	PoolHandle newHandle(0);

	ClearHandle(newHandle);

	// look up the name in our map
	ResourceNameMap::const_iterator iter = _nameMap.find(name);
	if (iter != _nameMap.end())
	{
		newHandle = iter->second;

		if (ValidHandle(newHandle))
		{
			ResourcePoolItem* pResource = GetResource(newHandle);
			return pResource;
		}
	}

	return 0;
}

inline ResourcePoolItem* ResourcePoolInterface::GetResource(PoolHandle handle)
{
	ResourcePoolItem* resource = InternalGetResource(handle);

	if (resource)
	{
		resource->AddRef();
	}

	return resource;
}

inline void ResourcePoolInterface::DestroyResource(ResourcePoolItem* pResource)
{
	if (pResource)
	{
		int32 referenceCount = pResource->GetCount();

		//debug_assert(referenceCount == 0, "destruction of non-zero reference count!");

		if (referenceCount <= 0)
		{
			PoolHandle handle = pResource->ResourceHandle();
			InternalDestroyResource(handle);
		}
	}
}


template <class T>
class ResourcePool : public ResourcePoolInterface
{
public:
	typedef T DataType;
	typedef DataPool<T> DataPool;

	// Creators...
	ResourcePool() {};
	~ResourcePool() {};

	// Base Class Overrides...
	void Initialize(uint16 growSize);
	void Destroy();
	bool IsInitialized()const;

	void DestroyAll();
	void DisableAll();
	void RestoreAll();
	void Clean(); // delete items no longer referenced

	DataType* CreateResource(const std::string &resourceName);
	DataType* LoadResource(const std::string &filename);
	DataType* GetResource(PoolHandle handle);
	DataType* FindResource(const std::string &name);

	// static data pool callbacks
	static void CallbackDestroy(DataPoolInterface* pPool, PoolHandle handle, void* resource);
	static void CallbackRestore(DataPoolInterface* pPool, PoolHandle handle, void* resource);
	static void CallbackDisable(DataPoolInterface* pPool, PoolHandle handle, void* resource);
	static void CallbackClean(DataPoolInterface* pPool, PoolHandle handle, void* resource);

private:

	// Data...
	DataPool _dataPool;

	// Private Functions...
	PoolHandle InternalCreateResource(const std::string &resourceName);
	void InternalDestroyResource(PoolHandle handle);
	ResourcePoolItem* InternalGetResource(PoolHandle handle);

	// Nonexistant Functions...

	ResourcePool(const ResourcePool& src);
	ResourcePool& operator=(const ResourcePool& src);
};

#endif

template<class T>
inline void ResourcePool<T>::Initialize(uint16 growSize)
{
	_dataPool.Initialize(growSize);
}

template<class T>
inline void ResourcePool<T>::Destroy()
{
	_dataPool.Destroy();
}

template<class T>
inline bool ResourcePool<T>::IsInitialized() const
{
	return _dataPool.IsInitialized();
}

template<class T>
inline void ResourcePool<T>::DestroyAll()
{
	_dataPool.ForEach(CallbackDestroy);
}

template<class T>
inline void ResourcePool<T>::DisableAll()
{
	_dataPool.ForEach(CallbackRestore);
}

template<class T>
inline void ResourcePool<T>::RestoreAll()
{
	_dataPool.ForEach(CallbackRestore);
}

template<class T>
inline void ResourcePool<T>::Clean()
{
	_dataPool.ForEach(CallbackClean);
}

template<class T>
inline T * ResourcePool<T>::CreateResource(const std::string & resourceName)
{
	return (T *)ResourcePoolInterface::CreateResource(resourceName);
}

template<class T>
inline T * ResourcePool<T>::LoadResource(const std::string & filename)
{
	return (T *)ResourcePoolInterface::LoadResource(filename);

}

template<class T>
inline T * ResourcePool<T>::GetResource(PoolHandle handle)
{
	return (T *)ResourcePoolInterface::GetResource(handle);

}

template<class T>
inline T * ResourcePool<T>::FindResource(const std::string & name)
{
	return (T *)ResourcePoolInterface::FindResource(name);
}

template<class T>
inline void ResourcePool<T>::CallbackDestroy(DataPoolInterface * pPool, PoolHandle handle, void * resource)
{
	((ResourcePoolItem *)resource)->DestroyResource();
}

template<class T>
inline void ResourcePool<T>::CallbackRestore(DataPoolInterface * pPool, PoolHandle handle, void * resource)
{
	((ResourcePoolItem *)resource)->RestoreResource();
}

template<class T>
inline void ResourcePool<T>::CallbackDisable(DataPoolInterface * pPool, PoolHandle handle, void * resource)
{
	((ResourcePoolItem *)resource)->DisableResource();
}

template<class T>
inline void ResourcePool<T>::CallbackClean(DataPoolInterface * pPool, PoolHandle handle, void * resource)
{
	if (((ResourcePoolItem *)resource)->GetCount() == 0)
	{
		pPool->Release(&handle);
	}
}

template<class T>
inline PoolHandle ResourcePool<T>::InternalCreateResource(const std::string & resourceName)
{
	PoolHandle handle(_dataPool.NextHandle());

	// make sure this name is not already in use
	//debug_assert(m_nameMap.find(resourceName) == m_nameMap.end(), "A resource with this name already exists");

	// add this resource name to our map of handles
	_nameMap[resourceName] = handle;

	// get a pointer to the resource and call it's creation function
	T* pResource = _dataPool.GetPtr(handle);
	pResource->CreateResource();

	return handle;
}

template<class T>
inline void ResourcePool<T>::InternalDestroyResource(PoolHandle handle)
{
	// get a pointer to the resource and call it's destruction handler
	T* pResource = _dataPool.GetPtr(handle);
	pResource->DestroyResource();

	// find the resource name and remove it from the map
	ResourceNameMap::iterator iter = _nameMap.begin();
	for (; iter != _nameMap.end(); ++iter)
	{
		if (handle == iter->second)
		{
			iter = _nameMap.erase(iter);
			break;
		}
	}

	// free the resource slot associated with the handle
	_dataPool.Release(&handle);
}

template<class T>
inline ResourcePoolItem * ResourcePool<T>::InternalGetResource(PoolHandle handle)
{
	return _dataPool.GetPtr(handle);
}

template<class T>
inline ResourcePool<T>::ResourcePool(const ResourcePool & src)
{
}

template<class T>
inline ResourcePool<T> & ResourcePool<T>::operator=(const ResourcePool & src)
{
	// TODO: insert return statement here
}
