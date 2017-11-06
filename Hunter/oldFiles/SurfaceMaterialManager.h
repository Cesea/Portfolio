#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "ResourceManager.h"
#include "SurfaceMaterial.h"

class TextureManager : public ResourceManager<Texture, TextureManager>
{
public:
	TextureManager(void);
	~TextureManager(void);
protected:
	//로드 방식만 재정의 
	Texture *LoadResource(const std::string &filePath, void* pParam = nullptr) override;
	virtual bool32 ChangeCurrentResource(Texture *pResource) override;

	//해재 방식만 재정의
	void ReleaseResource(Texture *pData) override;
	void ReleaseResource(const std::string &key) override;
};

class SurfaceMaterialManager : public ResourceManager<SurfaceMaterial, SurfaceMaterialManager>
{
public:
	SurfaceMaterialManager(void);
	~SurfaceMaterialManager(void);

protected:

	//로드 방식만 재정의 
	SurfaceMaterial *LoadResource(const std::string &filePath, void* pParam = nullptr) override;
	virtual bool32 ChangeCurrentResource(SurfaceMaterial *pResource) = 0;

	//해재 방식만 재정의
	void ReleaseResource(SurfaceMaterial *pData) override;
	void ReleaseResource(const std::string &key) override;
};

#define TEXTURE_MANAGER (TextureManager::GetInstance())
#define SURFACE_MANAGER (SurfaceMaterialManager::GetInstance())


#endif