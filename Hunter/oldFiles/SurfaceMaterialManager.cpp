#include "stdafx.h"
#include "SurfaceMaterialManager.h"

TextureManager::TextureManager(void)
{
}

TextureManager::~TextureManager(void)
{
	ClearResource();
}

Texture * TextureManager::LoadResource(const std::string &filePath, void* pParam )
{
	//텍스쳐 로딩
	Texture *pNewTexture = new Texture;

	if (FAILED(D3DXCreateTextureFromFile(gpDevice, filePath.c_str(), &pNewTexture->pTexture)))
	{
		Console::Log("Texture creation failed %s\n", filePath.c_str());
		return nullptr;
	}
	return pNewTexture;
}

//TODO : 여기서 디바이스 세팅을 해 주어야 할까???
bool32 TextureManager::ChangeCurrentResource(Texture * pResource)
{
	if (_pCurrentResource == pResource)
	{
		return false;
	}
	else
	{
		_pCurrentResource = pResource;
		return true;
	}
}

void TextureManager::ReleaseResource(Texture * pData)
{
	for (auto iter = _resources.begin(); iter != _resources.end(); ++iter)
	{
		if (iter->second->GetID() == pData->GetID())
		{
			COM_RELEASE(iter->second->pTexture);
			SAFE_DELETE(iter->second);
			_resources.erase(iter);
			break;
		}
	}
}

void TextureManager::ReleaseResource(const std::string & key)
{
	auto find = _resources.find(key);
	if (find == _resources.end())
	{
		//없음
		return;
	}
	else
	{
		COM_RELEASE(find->second->pTexture);
		SAFE_DELETE(find->second);
		_resources.erase(find);
	}
}

SurfaceMaterialManager::SurfaceMaterialManager(void)
{
}

SurfaceMaterialManager::~SurfaceMaterialManager(void)
{
}

SurfaceMaterial * SurfaceMaterialManager::LoadResource(const std::string & filePath, void * pParam)
{
	return nullptr;
}

bool32 SurfaceMaterialManager::ChangeCurrentResource(SurfaceMaterial * pResource)
{
	if (_pCurrentResource == pResource)
	{
		return false;
	}
	else
	{
		_pCurrentResource = pResource;
		return true;
	}
}

void SurfaceMaterialManager::ReleaseResource(SurfaceMaterial * pData)
{
}

void SurfaceMaterialManager::ReleaseResource(const std::string & key)
{
}
