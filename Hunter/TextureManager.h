#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "ResourceManager.h"

class TextureManager : public ResourceManager<IDirect3DTexture9, TextureManager>
{
public:
	TextureManager(void);
	~TextureManager(void);

protected:

	//�ε� ��ĸ� ������ 
	IDirect3DTexture9 *LoadResource(const std::string &filePath, void* pParam = nullptr) override;

	//���� ��ĸ� ������
	void ReleaseResource(IDirect3DTexture9 *pData) override;
	void ReleaseResource(const std::string &key) override;
};

#define TEXTURE_MANAGER (TextureManager::GetInstance())


#endif