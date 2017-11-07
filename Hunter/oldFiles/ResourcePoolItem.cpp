#include "stdafx.h"
#include "ResourcePoolItem.h"
#include "ResourcePool.h"


// ResourceCode ///////////////////////////////////////////////////

ResourceCode::ResourceCode()
	:value(INVALID_CODE)
{

}

ResourceCode::ResourceCode(const ResourceCode &other)
{
	value = other.value;
}

ResourceCode::ResourceCode(uint32 source)
{
	value = source;
}

ResourceCode::ResourceCode(uint16 family, uint16 type)
{
	this->family = family;
	this->type = type;
}
ResourceCode::~ResourceCode()
{
}

//bool ResourceCode::operator< (const ResourceCode &other)
//{
//	return value < other.value;
//}

//Resource Pool Item //////////////////////////////////////////////////
ResourcePoolItem::ResourcePoolItem()
	:_resourceCode(0), _resourceHandle(0), _resourceFlags(0), _pResourcePool(nullptr)
{
}

ResourcePoolItem::~ResourcePoolItem()
{
}

ResourceCode ResourcePoolItem::GetResourceCode() const
{
	return _resourceCode;
}

const ResourcePoolInterface *ResourcePoolItem::GetResourcePool() const
{
	return _pResourcePool;
}

PoolHandle ResourcePoolItem::GetResourceHandle() const
{
	return _resourceHandle;
}

Uint32Flags ResourcePoolItem::GetResourceFlags() const
{
	return _resourceFlags;
}

bool32 ResourcePoolItem::IsResourceCreated() const
{
	return TEST_BIT(_resourceFlags, ResourceState::Created);
}

bool32 ResourcePoolItem::IsResourceDisabled() const
{
	return TEST_BIT(_resourceFlags, ResourceState::Disabled);
}

bool32 ResourcePoolItem::IsResourceLoaded() const
{
	return TEST_BIT(_resourceFlags, ResourceState::Loaded);
}

const std::string &ResourcePoolItem::FindResourceName() const
{
	if (_pResourcePool)
	{
		return *_pResourcePool->FindResourceName(_resourceHandle);
	}
	return std::string();
}

void ResourcePoolItem::SetResourceName(const std::string &name)
{
	if (_pResourcePool)
	{
		_pResourcePool->SetResourceName(_resourceHandle, name);
	}
}

void ResourcePoolItem::SetAlteredFlag(bool b)
{
	_resourceFlags.SetBit(ResourceState::Altered, b);
}

bool32 ResourcePoolItem::IsAlteredFlag() const
{
	return TEST_BIT(_resourceFlags, ResourceState::Altered);
}

int32 ResourcePoolItem::Release()
{
	return 0;
}

void ResourcePoolItem::SetResourceCode(const ResourceCode code)
{
	_resourceCode = code;
}

void ResourcePoolItem::SetResourcePool(ResourcePoolInterface *pPool)
{
	_pResourcePool = pPool;
}

void ResourcePoolItem::SetResourceHandle(PoolHandle handle)
{
	_resourceHandle = handle;
}

void ResourcePoolItem::SetResourceFlags(int32 flagbit, bool setting)
{
	_resourceFlags.SetBit(flagbit, setting);
}

void ResourcePoolItem::NotifyCreated()
{
	SetResourceFlags(ResourceState::Created, true);
}

void ResourcePoolItem::NotifyDestroyed()
{
	SetResourceFlags(ResourceState::Created, false);
}

void ResourcePoolItem::NotifyDisabled()
{
	SetResourceFlags(ResourceState::Disabled, true);
}

void ResourcePoolItem::NotifyLoaded()
{
	SetResourceFlags(ResourceState::Loaded, true);
}

void ResourcePoolItem::NotifyUnloaded()
{
	SetResourceFlags(ResourceState::Loaded, false);
}

void ResourcePoolItem::NotifyRestore()
{
	SetResourceFlags(ResourceState::Disabled, false);
}

void ResourcePoolItem::NotifySaved()
{
	SetAlteredFlag(false);
}

//not used
ResourcePoolItem::ResourcePoolItem(const ResourcePoolItem &other)
{

}
ResourcePoolItem &ResourcePoolItem::operator= (const ResourcePoolItem &other)
{
	return *this;
}
