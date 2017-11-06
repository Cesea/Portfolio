#include "stdafx.h"
#include "ResourcePoolItem.h"


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

bool ResourceCode::operator< (const ResourceCode &other)
{
	return value < other.value;
}

//Resource Pool Item //////////////////////////////////////////////////


ResourcePoolItem::ResourcePoolItem()
{
}

ResourcePoolItem::~ResourcePoolItem()
{
}

ResourceCode ResourcePoolItem::GetResourceCode() const
{
	return ResourceCode();
}

ResourcePoolInterface *ResourcePoolItem::GetResourcePool() const
{
	return false;
}

PoolHandle ResourcePoolItem::GetHandle() const
{
	return false;
}

Uint32Flags ResourcePoolItem::GetFlags() const
{
	return Uint32Flags();
}

bool32 ResourcePoolItem::IsResourceCreated() const
{
	return false;
}

bool32 ResourcePoolItem::IsResourceDisabled() const
{
	return false;
}

bool32 ResourcePoolItem::IsResourceLoaded() const
{
	return false;
}

const std::string ResourcePoolItem::FindResourceName() const
{
	return std::string();
}

void ResourcePoolItem::SetResourceName(const std::string &name)
{
}

void ResourcePoolItem::SetAlteredFlag(bool b)
{
}

bool32 ResourcePoolItem::IsAlteredFlag() const
{
	return false;
}

int32 ResourcePoolItem::Release()
{
	return 0;
}

void ResourcePoolItem::SetResourceCode(const ResourceCode code)
{
}
void ResourcePoolItem::SetResourcePool(const ResourcePoolInterface *pPool)
{
}
void ResourcePoolItem::SetHandle(PoolHandle handle)
{
}
void ResourcePoolItem::SetFlags(int flagbit, bool setting)
{
}

void ResourcePoolItem::NotifyCreated()
{
}

void ResourcePoolItem::NotifyDisabled()
{
}

void ResourcePoolItem::NotifyLoaded()
{
}

void ResourcePoolItem::NotifyUnloaded()
{
}

void ResourcePoolItem::NotifyRestore()
{
}

void ResourcePoolItem::NotifyDestroyed()
{
}

void ResourcePoolItem::NotifySaved()
{
}

ResourcePoolItem::ResourcePoolItem(const ResourcePoolItem &other)
{

}
ResourcePoolItem &ResourcePoolItem::operator= (const ResourcePoolItem &other)
{
	return *this;
}
