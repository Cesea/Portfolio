#include "stdafx.h"
#include "bat.h"

#define DESC_FLIP (0)
#define DESC_ATTACK1 (1)
#define DESC_ATTACK2 (2)
#define DESC_ATTACK3 (3)

void Bat::SetupCallbackAndCompression()
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	_callbackData[0]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[0]._pPosition = &refTransform._position;
	_callbackData[0]._description = DESC_FLIP;

	_callbackData[1]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[1]._pPosition = &refTransform._position;
	_callbackData[1]._description = DESC_ATTACK1;

	_callbackData[2]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[2]._pPosition = &refTransform._position;
	_callbackData[2]._description = DESC_ATTACK2;

	_callbackData[3]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[3]._pPosition = &refTransform._position;
	_callbackData[3]._description = DESC_ATTACK3;

	ID3DXAnimationController *pController = _pActionComp->_pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();

	//FORWARD
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_FORWARD], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_FLIP];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

#pragma region ATTACK
	//ATTACK1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_ATTACK1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.10) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[1].Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//ATTACK2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_ATTACK2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.07) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[1].Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//ATTACK3
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_ATTACK3], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[3];
		keys[0].Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[1].Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_ATTACK3];

		keys[2].Time = (anim->GetPeriod() * 0.73) * anim->GetSourceTicksPerSecond();
		keys[2].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

#pragma endregion

	//HIT
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[3];
		keys[0].Time = (anim->GetPeriod() * 0.07) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[1].Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		keys[2].Time = (anim->GetPeriod() * 0.49) * anim->GetSourceTicksPerSecond();
		keys[2].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//DEATH
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_FLIP];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//ROAR
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_ROAR], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[4];
		keys[0].Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[1].Time = (anim->GetPeriod() * 0.36) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		keys[2].Time = (anim->GetPeriod() * 0.40) * anim->GetSourceTicksPerSecond();
		keys[2].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		keys[3].Time = (anim->GetPeriod() * 0.72) * anim->GetSourceTicksPerSecond();
		keys[3].pCallbackData = (void *)&_callbackData[DESC_FLIP];

		AddCallbackKeysAndCompress(pController, anim, 4, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

}


HRESULT BatCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CatCallbackData* pData = (CatCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case BAT_ATTACK1:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("bat_spit_01", *pData->_pPosition);
		}
	}break;
	case BAT_ATTACK2:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK2)
		{
			SOUNDMANAGER->Play3D("bat_attack_01", *pData->_pPosition);
		}
	}break;
	case BAT_ATTACK3:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK3)
		{
			SOUNDMANAGER->Play3D("bat_attack_02", *pData->_pPosition);
		}
	}break;
	case BAT_DEATH:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_dead_01", *pData->_pPosition);
		}
	}break;
	case BAT_FORWARD:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
	}break;
	case BAT_IDLE:
	{
	}break;
	case BAT_HIT1:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("bat_hit_01", *pData->_pPosition);
		}
	}break;
	case BAT_ROAR:
	{
		if (pData->_description == DESC_FLIP)
		{
			SOUNDMANAGER->Play3D("bat_forward_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("bat_roar_01", *pData->_pPosition);
		}
	}break;
	}
}
