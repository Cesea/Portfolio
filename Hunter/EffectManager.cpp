#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

EffectHandle EffectManager::Create(const std::string & fileName)
{
	EffectHandle handle = GetNextFreeHandle();
	DWORD shaderFlag = 0;
#if defined (DEBUG) || defined (_DEBUG)
	shaderFlag |= D3DXSHADER_DEBUG;
#endif
	if (_datas[handle].Create(fileName, shaderFlag))
	{
		_datas[handle].SetHandle(handle);
		_currentSize++;
		return handle;
	}
	return INVALID_HANDLE;
}

bool32 EffectManager::Destroy(EffectHandle handle)
{
	Assert(handle < _currentSize);
	_datas[handle].Destroy();
	return true;
}
