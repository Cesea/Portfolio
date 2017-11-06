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
	//�ε� ��ĸ� ������ 
	Texture *LoadResource(const std::string &filePath, void* pParam = nullptr) override;
	virtual bool32 ChangeCurrentResource(Texture *pResource) override;

	//���� ��ĸ� ������
	void ReleaseResource(Texture *pData) override;
	void ReleaseResource(const std::string &key) override;
};

class SurfaceMaterialManager : public ResourceManager<SurfaceMaterial, SurfaceMaterialManager>
{
public:
	SurfaceMaterialManager(void);
	~SurfaceMaterialManager(void);

protected:

	//�ε� ��ĸ� ������ 
	SurfaceMaterial *LoadResource(const std::string &filePath, void* pParam = nullptr) override;
	virtual bool32 ChangeCurrentResource(SurfaceMaterial *pResource) = 0;

	//���� ��ĸ� ������
	void ReleaseResource(SurfaceMaterial *pData) override;
	void ReleaseResource(const std::string &key) override;
};

#define TEXTURE_MANAGER (TextureManager::GetInstance())
#define SURFACE_MANAGER (SurfaceMaterialManager::GetInstance())


#endif