#include "stdafx.h"
#include "ResourcePoolItem.h"

#include "ResourcePool.h"

inline const std::string & ResourcePoolItem::FindResourceName() const
{
	if (_pResourcePool)
	{
		return _pResourcePool->FindResourceName(_resourceHandle);
	}
	return std::string();
}
inline void ResourcePoolItem::SetResourceName(const char * name)
{
	if (_pResourcePool)
	{
		return _pResourcePool->SetResourceName(_resourceHandle, name);
	}
}
inline int32 ResourcePoolItem::Release()
{
	int32 refCount = ReferenceCounter::Release();
	if (refCount == 0)
	{
		if (_pResourcePool)
		{
			_pResourcePool->DestroyResource(this);
		}
	}
	return refCount;
}