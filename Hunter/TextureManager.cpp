#include "stdafx.h"
#include "TextureManager.h"

TextureManager::TextureManager(void)
{
}

TextureManager::~TextureManager(void)
{
	ClearResource();
}

IDirect3DTexture9 * TextureManager::LoadResource(const std::string &filePath, void* pParam )
{
	//텍스쳐 로딩
	IDirect3DTexture9 *pNewTexture = nullptr;
	if( FAILED( D3DXCreateTextureFromFile( gpDevice, filePath.c_str(), &pNewTexture ) ) )
		return nullptr;

	return pNewTexture;
}

void TextureManager::ReleaseResource(IDirect3DTexture9 * pData)
{
	COM_RELEASE(pData);
}

void TextureManager::ReleaseResource(const std::string & key)
{
	auto find = _resources.find(key);
	if (find == _resources.end())
	{
		return;
	}
	else
	{
		COM_RELEASE(find->second);
		_resources.erase(find);
	}
}
