#include "stdafx.h"
#include "ResourcePoolManager.h"

#include "ResourcePool.h"

ResourcePoolManager::ResourcePoolManager()
{
	// paranoid pre-cleaning of STL collections
	_resourceTypeMap.clear();
	for (int i = 0; i < ResourceFamily::eCount; ++i)
	{
		_resourceFamilyList[i].clear();
	}
}

void ResourcePoolManager::RegisterResourcePool(ResourceCode code, ResourcePoolInterface * pInterface)
{
	//debug_assert(pInterface, "invalid cResourcePoolInterface pointer");
	//debug_assert(code.family >= 0, "invalid family index");
	//debug_assert(code.family < k_nTotalResourceFamilies, "invalid family index");
	//debug_assert(m_resourceTypeMap.find(code) == m_resourceTypeMap.end(), "Resource type code already registered");

	_resourceTypeMap[code] = pInterface;
	_resourceFamilyList[code.family].push_back(pInterface);
}

ResourcePoolInterface * ResourcePoolManager::UnregisterResourcePool(ResourceCode code)
{
	//debug_assert(code.family >= 0, "invalid family index");
	//debug_assert(code.family < k_nTotalResourceFamilies, "invalid family index");

	ResourcePoolInterface* pInterface = 0;
	ResourcePoolMap::iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		pInterface = (*iter).second;
		_resourceTypeMap.erase(iter);
	}

	if (pInterface)
	{
		for (ResourcePoolFamilyList::iterator listIter = _resourceFamilyList[code.family].begin();
			listIter != _resourceFamilyList[code.family].end();
			++listIter)
		{
			if ((*listIter) == pInterface)
			{
				_resourceFamilyList[code.family].erase(listIter);
				return(pInterface);
			}
		}
	}
	return(pInterface);
}

void ResourcePoolManager::DestroyAll()
{
	for (int i = 0; i < ResourceFamily::eCount; ++i)
	{
		DestroyResourceFamily(i);
	}
}

void ResourcePoolManager::DisableAll()
{
	for (int i = 0; i < ResourceFamily::eCount; ++i)
	{
		DisableResourceFamily(i);
	}
}

void ResourcePoolManager::RestoreAll()
{
	for (int i = 0; i<ResourceFamily::eCount; ++i)
	{
		RestoreResourceFamily(i);
	}
}

void ResourcePoolManager::Clean()
{
	for (int i = 0; i<ResourceFamily::eCount; ++i)
	{
		CleanResourceFamily(i);
	}
}

void ResourcePoolManager::DestroyResourceFamily(int32 family)
{
	//debug_assert(family >= 0, "invalid family index");
	//debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->DestroyAll();
	}
}

void ResourcePoolManager::DisableResourceFamily(int32 family)
{
	//debug_assert(family >= 0, "invalid family index");
	//debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->DisableAll();
	}
}

void ResourcePoolManager::RestoreResourceFamily(int32 family)
{
	//debug_assert(family >= 0, "invalid family index");
	//debug_assert(family < k_nTotalResourceFamilies, "invalid family index");
	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->RestoreAll();
	}
}

void ResourcePoolManager::CleanResourceFamily(int32 family)
{
	//debug_assert(family >= 0, "invalid family index");
	//debug_assert(family < k_nTotalResourceFamilies, "invalid family index");

	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->Clean();
	}
}

void ResourcePoolManager::DestroyResourceType(ResourceCode code)
{
	ResourcePoolMap::iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		(*iter).second->DestroyAll();
	}
}

void ResourcePoolManager::DisableResourceType(ResourceCode code)
{
	// find the code in our map
	ResourcePoolMap::iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		(*iter).second->DisableAll();
	}
}

void ResourcePoolManager::RestoreResourceType(ResourceCode code)
{
	ResourcePoolMap::iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		(*iter).second->RestoreAll();
	}
}

void ResourcePoolManager::CleanResourceType(ResourceCode code)
{
	// find the code in our map
	ResourcePoolMap::iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		(*iter).second->Clean();
	}
}

ResourcePoolInterface * ResourcePoolManager::FindResourcePool(ResourceCode code) const
{
	ResourcePoolInterface* result = 0;
	ResourcePoolMap::const_iterator iter = _resourceTypeMap.find(code);
	if (iter != _resourceTypeMap.end())
	{
		result = (*iter).second;
	}
	return result;
}

PoolHandle ResourcePoolManager::FindResourceHandle(ResourceCode code, const std::string & name) const
{
	ResourcePoolInterface* pPool = FindResourcePool(code);
	PoolHandle handle{};

	ClearHandle(handle);

	if (pPool)
	{
		handle = pPool->FindResourceHandle(name);
	}

	return handle;
}

ResourcePoolItem * ResourcePoolManager::FindResource(ResourceCode code, const std::string & name) const
{
	ResourcePoolInterface* pPool = FindResourcePool(code);
	ResourcePoolItem* result = 0;

	if (pPool)
	{
		PoolHandle handle = pPool->FindResourceHandle(name);
		if (ValidHandle(handle))
		{
			result = pPool->GetResource(handle);
		}
	}

	return result;
}

ResourcePoolItem * ResourcePoolManager::FindResource(ResourceCode code, PoolHandle handle) const
{
	ResourcePoolInterface* pPool = FindResourcePool(code);
	ResourcePoolItem* result = 0;

	if (pPool)
	{
		if (ValidHandle(handle))
		{
			result = pPool->GetResource(handle);
		}
	}

	return result;
}
