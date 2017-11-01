#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include "ResourceManager.h"

#include "StaticMesh.h"

class StaticMeshManager : public ResourceManager<Effect, StaticMesh>
{
public:
	StaticMeshManager(void);
	~StaticMeshManager(void);

	virtual Effect *LoadResource(const std::string &filePath, void* pParam = NULL);

	virtual void ReleaseResource(StaticMesh *pData);
	virtual void ReleaseResource(const std::string &key);

};

#define STATICMESHMANAGER (StaticMeshManager::GetInstance())


#endif