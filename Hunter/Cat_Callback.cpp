#include "stdafx.h"
#include "Cat.h"

#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Cat::SetupCallbackAndCompression()
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	_callbackData[0]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[0]._pPosition = &refTransform._position;
	_callbackData[0]._description = DESC_LEFT;

	_callbackData[1]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[1]._pPosition = &refTransform._position;
	_callbackData[1]._description = DESC_RIGHT;

	_callbackData[2]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[2]._pPosition = &refTransform._position;
	_callbackData[2]._description = DESC_ATTACK1;

	_callbackData[3]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[3]._pPosition = &refTransform._position;
	_callbackData[3]._description = DESC_ATTACK2;

	_callbackData[4]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[4]._pPosition = &refTransform._position;
	_callbackData[4]._description = DESC_ATTACK3;

	_callbackData[5]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[5]._pPosition = &refTransform._position;
	_callbackData[5]._description = DESC_ATTACK4;

	ID3DXAnimationController *pController = _pActionComp->_pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();

#pragma region Roar
	//Roar
	//{
	//	ID3DXKeyframedAnimationSet *anim;
	//	pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAR_ROAR], (ID3DXAnimationSet **)&anim);

	//	D3DXKEY_CALLBACK keys;
	//	keys.Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
	//	keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

	//	AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	//}

#pragma endregion

#pragma region Run
	//Run
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_RUN], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.15) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		keys[1].Time = (anim->GetPeriod() * 0.80) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion
	//Hit
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.13) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//Dead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.10) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}


#pragma region Attack
	//Attack1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_ATTACK1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//Attack2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_ATTACK2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//Attack4
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_ATTACK5], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

#pragma endregion

}

HRESULT CatCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CatCallbackData* pData = (CatCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case CAT_ATTACK1 :
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("cat_attack_01", *pData->_pPosition);
		}
	}break;
	case CAT_ATTACK2:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("cat_attack_02", *pData->_pPosition);
		}
	}break;
	case CAT_ATTACK3:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("cat_attack_03", *pData->_pPosition);
		}
	}break;
	case CAT_ATTACK4:
	{
	}break;
	case CAT_ATTACK5:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("cat_attack_04", *pData->_pPosition);
		}
	}break;
	case CAT_DEATH:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("cat_dead_01", *pData->_pPosition);
		}
	}break;
	case CAT_HIT1:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("cat_hit_01", *pData->_pPosition);
		}
	}break;

	case CAT_IDLE:
	{
	}break;

	case CAT_RUN:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("cat_run_01", *pData->_pPosition);
		}

	}break;
	case CAT_STAND:
	{
	}break;
	case CAT_WALK:
	{
	}break;
	}


	return S_OK;
}
