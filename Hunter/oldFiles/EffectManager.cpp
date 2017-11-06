#include "stdafx.h"
#include "EffectManager.h"

EffectManager::EffectManager(void)
{
}

EffectManager::~EffectManager(void)
{
}

Effect * EffectManager::LoadResource(const std::string & filePath, void * pParam)
{
	//effect loading
	Effect *pNewEffect = nullptr;
	DWORD shaderFlag = 0;

	#if defined (DEBUG) | defined (_DEBUG)
		shaderFlag |= shaderFlag;
	#endif

	if (!pNewEffect->Load(filePath, shaderFlag))
	{
		return nullptr;
	}
	return pNewEffect;
}

void EffectManager::ReleaseResource(Effect * pData)
{
	//resource안의 요소도 지워야 하지 않을까
	SAFE_DELETE(pData);
}

void EffectManager::ReleaseResource(const std::string & key)
{
	auto find = _resources.find(key);
	if (find == _resources.end())
	{
		return;
	}
	else
	{
		SAFE_DELETE(find->second);
		_resources.erase(find);
	}
}

bool32 EffectManager::ChangeCurrentResource(Effect * pData)
{
	if (_pCurrentResource == pData)
	{
		return false;
	}
	else
	{
		_pCurrentResource = pData;
		return true;
	}
}
