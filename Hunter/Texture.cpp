#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool32 Texture::Create(const std::string & fileName)
{
	D3DXIMAGE_INFO imageInfo;
	//이미지의 정보르르 가져올 수 없다
	if (FAILED(D3DXGetImageInfoFromFile(fileName.c_str(), &imageInfo)))
	{
		return false;
	}

	_width = imageInfo.Width;
	_height = imageInfo.Height;

	if (FAILED(D3DXCreateTextureFromFile(gpDevice, fileName.c_str(), &_pTexture)))
	{
		return false;
	}
	return true;
}

bool32 Texture::Create(int32 width, int32 height, void * pData)
{
	_width = width;
	_height = _height;
	
	//if (pData == nullptr)
	//{
	//	if (FAILED(D3DXCreateTexture(gpDevice, _width, _height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_pTexture)))
	//	{
	//		return false;
	//	}
	//}
	//else
	//{

	//}
	return true;
}

bool32 Texture::Destroy()
{
	COM_RELEASE(_pTexture);
	return true;
}
