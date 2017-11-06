#ifndef RESOURCE_POOL_MANAGER_H
#define RESOURCE_POOL_MANAGER_H

#include "ResourcePoolItem.h"

#include "SingletonBase.h"
#include <map>
#include <list>

#if defined FindResource
	#undef FindResource
#endif

enum ResourceFamilyType
{
	VideoResource = 0,
	AudioResource,
	GameResource,
	RESOURCE_FAMILY_COUNT
};

enum VideoResourceType
{
	TextureResource = 0,
	VBufferResource,
	IBufferResource,
	RenderResource,
	RenderSetResource,
	ModelResource,
	ImageResource,
	SMaterialResource,
	VIDEO_RESOURCE_COUNT
};

enum AudioResourceType
{

};

enum GameResourceType
{

};

class ResourcePoolInterface;

#define RESOURCEMANAGER	(ResourcePoolManager::GetInstance())

class ResourcePoolManager : public SingletonBase<ResourcePoolManager>
{
public :
	typedef std::map<ResourceCode, ResourcePoolInterface *> ResourcePoolTypeMap;
	typedef std::list<ResourcePoolInterface *> ResourcePoolFamilyList;
	// Creators...
	ResourcePoolManager();
	~ResourcePoolManager() {};

	// registration of resource manager interfaces
	void RegisterResourcePool(ResourceCode code, ResourcePoolInterface* pInterface);
	ResourcePoolInterface*  UnregisterResourcePool(ResourceCode code);

	// operations for all resource types
	void DestroyAll();
	void DisableAll();
	void RestoreAll();
	void Clean(); // delete items no longer referenced

	// operations on specific resource types
	void DestroyResourceFamily(int32 family);
	void DisableResourceFamily(int32 family);
	void RestoreResourceFamily(int32 family);
	void CleanResourceFamily(int32 family);

	void DestroyResourceType(ResourceCode code);
	void DisableResourceType(ResourceCode code);
	void RestoreResourceType(ResourceCode code);
	void CleanResourceType(ResourceCode code);

	ResourcePoolInterface* FindResourcePool(ResourceCode code) const;
	PoolHandle FindResourceHandle(ResourceCode code, const std::string& name) const;
	ResourcePoolItem* FindResource(ResourceCode code, const std::string& name) const;
	ResourcePoolItem* FindResource(ResourceCode code, PoolHandle handle) const;

private:

	ResourcePoolFamilyList _resourceFamilyList[RESOURCE_FAMILY_COUNT];
	ResourcePoolTypeMap	_resourceTypeMap;
};

#endif