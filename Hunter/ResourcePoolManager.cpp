#include "stdafx.h"
#include "ResourcePoolManager.h"

#include "ResourcePool.h"

ResourcePoolManager::ResourcePoolManager()
{
	_resourceTypeMap.clear();
	for (int32 i = 0; i < RESOURCE_FAMILY_COUNT; ++i)
	{
		_resourceFamilyList[i].clear();
	}
}

void ResourcePoolManager::RegisterResourcePool(ResourceCode code, ResourcePoolInterface * pInterface)
{
	auto find = _resourceTypeMap.find(code);
	//¾øÀ¸¸é
	if (find == _resourceTypeMap.end())
	{
		_resourceTypeMap.insert(code, pInterface);
		_resourceFamilyList[code.family].push_back(pInterface);
	}
}

ResourcePoolInterface * ResourcePoolManager::UnregisterResourcePool(ResourceCode code)
{
	ResourcePoolInterface* pInterface = 0;
	ResourcePoolTypeMap::iterator iter = _resourceTypeMap.find(code);
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
	for (int32 i = 0; i < RESOURCE_FAMILY_COUNT; ++i)
	{
		DestroyResourceFamily(i);
	}
}

void ResourcePoolManager::DisableAll()
{
	for (int i = 0; i < RESOURCE_FAMILY_COUNT; ++i)
	{
		DisableResourceFamily(i);
	}
}

void ResourcePoolManager::RestoreAll()
{
	for (int i = 0; i < RESOURCE_FAMILY_COUNT; ++i)
	{
		RestoreResourceFamily(i);
	}
}

void ResourcePoolManager::Clean()
{
	for (int i = 0; i < RESOURCE_FAMILY_COUNT; ++i)
	{
		CleanResourceFamily(i);
	}
}

void ResourcePoolManager::DestroyResourceFamily(int32 family)
{
	for(ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->DestroyAll();
	}
}

void ResourcePoolManager::DisableResourceFamily(int32 family)
{
	for(ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->DisableAll();
	}
}

void ResourcePoolManager::RestoreResourceFamily(int32 family)
{
	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->RestoreAll();
	}
}

void ResourcePoolManager::CleanResourceFamily(int32 family)
{
	for (ResourcePoolFamilyList::iterator iter = _resourceFamilyList[family].begin();
		iter != _resourceFamilyList[family].end();
		++iter)
	{
		(*iter)->Clean();
	}
}

void ResourcePoolManager::DestroyResourceType(ResourceCode code)
{
	auto find = _resourceTypeMap.find(code);
	if (find != _resourceTypeMap.end())
	{
		(*iter).second->DestroyAll();
	}
}

void ResourcePoolManager::DisableResourceType(ResourceCode code)
{
	auto find = _resourceTypeMap.find(code);
	if (find != _resourceTypeMap.end())
	{
		(*iter).second->DisableAll();
	}
}

void ResourcePoolManager::RestoreResourceType(ResourceCode code)
{
	auto find = _resourceTypeMap.find(code);
	if (find != _resourceTypeMap.end())
	{
		(*iter).second->RestoreAll();
	}
}

void ResourcePoolManager::CleanResourceType(ResourceCode code)
{
	auto find = _resourceTypeMap.find(code);
	if (find != _resourceTypeMap.end())
	{
		(*iter).second->Clean();
	}
}

ResourcePoolInterface * ResourcePoolManager::FindResourcePool(cResourceCode code) const
{
	ResourcePoolInterface *result = nullptr;
	auto find = _resourceTypeMap.find(code);
	if (find != _resourceTypeMap.end())
	{
		result = (*find).second;
	}
	return result;
}

ResourcePoolInterface * ResourcePoolManager::FindResourcePool(ResourceCode code) const
{
	return nullptr;
}

PoolHandle ResourcePoolManager::FindResourceHandle(ResourceCode code, const std::string & name) const
{
	ResourcePoolInterface* pool = FindResourcePool(code);
	PoolHandle handle;

	ClearHandle(handle);

	if (pool)
	{
		handle = pool->FindResourceHandle(Name);
	}

	return handle;
}

ResourcePoolItem * ResourcePoolManager::FindResource(ResourceCode code, const std::string & name) const
{
	ResourcePoolInterface* pool = FindResourcePool(code);
	ResourcePoolItem* result = 0;

	if (pool)
	{
		PoolHandle handle = pool->FindResourceHandle(name);
		if (HandleValid(handle))
		{
			result = pool->GetResource(handle);
		}
	}
	return result;
}

ResourcePoolItem * ResourcePoolManager::FindResource(ResourceCode code, PoolHandle handle) const
{
	ResourcePoolInterface* pool = FindResourcePool(code);
	ResourcePoolItem* result = 0;
	if (pool)
	{
		if (HandleValid(handle))
		{
			result = pool->GetResource(handle);
		}
	}
	return result;
}
