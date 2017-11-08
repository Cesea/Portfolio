#include "stdafx.h"
#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

TextureHandle TextureManager::CreateFromFile(const std::string & fileName)
{
	TextureHandle currentHandle = GetNextFreeHandle();
	if (_datas[currentHandle].Create(fileName))
	{
		_datas[currentHandle].SetHandle(currentHandle);
		return currentHandle;
	}
	return INVALID_HANDLE;
}

//미완성 함수임... 안을 채워야 한다.
TextureHandle TextureManager::CreateFromMemory(int32 width, int32 height, void * pData)
{
	TextureHandle currentHandle = GetNextFreeHandle();
	if (_datas[currentHandle].Create(width, height, pData))
	{
		_datas[currentHandle].SetHandle(currentHandle);
		return currentHandle;
	}
	return INVALID_HANDLE;
}

bool32 TextureManager::Destroy(ResourceHandle handle)
{
	return bool32();
}
