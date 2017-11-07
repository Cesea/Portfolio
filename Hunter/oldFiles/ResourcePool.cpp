#include "stdafx.h"
#include "ResourcePool.h"

ResourcePoolInterface::ResourcePoolInterface()
	:_registrationCode(ResourceCode::INVALID_CODE)
{
	_nameTable.clear();
}

void ResourcePoolInterface::Initialize()
{
}

void ResourcePoolInterface::Destroy()
{
}

bool ResourcePoolInterface::IsInitialized() const
{
	return false;
}

void ResourcePoolInterface::DestroyAll()
{
}

void ResourcePoolInterface::DisableAll()
{
}

void ResourcePoolInterface::RestoreAll()
{
}

void ResourcePoolInterface::Clean()
{
}

ResourcePoolItem * ResourcePoolInterface::CreateResource(const std::string & resourceName)
{
	PoolHandle handle = InternalCreateResource(resourceName);

	if (HandleValid(handle))
	{
		ResourcePoolItem* pResource = GetResource(handle);

		pResource->SetResourcePool(this);
		pResource->SetResourceHandle(handle);
		pResource->SetResourceCode(_registrationCode);
		return pResource;
	}

	return nullptr;
}

ResourcePoolItem * ResourcePoolInterface::LoadResource(const std::string & filename)
{
	return nullptr;
}

ResourcePoolItem * ResourcePoolInterface::GetResource(PoolHandle handle)
{
	return nullptr;
}

ResourcePoolItem * ResourcePoolInterface::FindResource(const std::string & name)
{
	return nullptr;
}

void ResourcePoolInterface::DestroyResource(ResourcePoolItem * pResource)
{
}

bool ResourcePoolInterface::SaveResource(ResourcePoolItem * pResource)
{
	return false;
}

PoolHandle ResourcePoolInterface::FindResourceHandle(const std::string & name)
{
	PoolHandle newHandle(0);

	ClearHandle(newHandle);

	// look up the name in our map
	NameTable::const_iterator iter = _nameTable.find(name);
	if (iter != _nameTable.end())
	{
		newHandle = iter->second;
	}

	return newHandle;
}

const std::string * ResourcePoolInterface::FindResourceName(PoolHandle handle) const
{
	NameTable::const_iterator iter = _nameTable.begin();
	for (; iter != _nameTable.end(); ++iter)
	{
		if (handle == iter->second)
		{
			return &iter->first;
		}
	}
	return nullptr;
}

void ResourcePoolInterface::SetResourceName(PoolHandle handle, const std::string &name)
{
	NameTable::iterator iter = _nameTable.begin();
	for (; iter != _nameTable.end(); ++iter)
	{
		if (handle == iter->second)
		{
			iter = _nameTable.erase(iter);
			break;
		}
	}

	_nameTable[name] = handle;
}

void ResourcePoolInterface::RegisterResourcePool(ResourceCode code)
{
	_registrationCode = code;
	RESOURCEMANAGER->RegisterResourcePool(_registrationCode, this);
}

void ResourcePoolInterface::UnregisterResourcePool()
{
	RESOURCEMANAGER->UnregisterResourcePool(_registrationCode);
	_registrationCode.SetInvalid();
}

ResourceCode ResourcePoolInterface::RegistrationCode() const
{
	return _registrationCode;
}
