#include "stdafx.h"
#include "Hydra.h"

#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Hydra::SetupCallbackAndCompression()
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

#pragma region Walk
	//Walk
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_RUN], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.06) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

		keys[1].Time = (anim->GetPeriod() * 0.50) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Bite
	//Bite1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BITE1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.3) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Bite2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BITE2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.3) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

	//Bite3
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BITE3], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.3) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK3];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Breath
	//Breath1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BREATH_FIRE1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.3) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Breath2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BREATH_FIRE2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.2) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Breath3
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_BREATH_FIRE3], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.2) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK3];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region WHIP
	//WhipHead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_WHIP_HEAD], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.30) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//WhipTail
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_WHIP_TAIL], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.32) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

#pragma endregion

#pragma region Hit
	//Hit1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.10) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Dead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.10) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		keys[1].Time = (anim->GetPeriod() * 0.40) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

	//Roar
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_IDLE], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.44) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
}

HRESULT HydraCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	HydraCallbackData* pData = (HydraCallbackData *)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case HYDRA_BITE1:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("hydra_attack_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_BITE2:
	{
		if (pData->_description == DESC_ATTACK2)
		{
			SOUNDMANAGER->Play3D("hydra_attack_02", *pData->_pPosition);
		}
	}break;
	case HYDRA_BITE3:
	{
		if (pData->_description == DESC_ATTACK3)
		{
			SOUNDMANAGER->Play3D("hydra_attack_03", *pData->_pPosition);
		}
	}break;
	case HYDRA_BREATH_FIRE1:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("hydra_breath_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_BREATH_FIRE2:
	{
		if (pData->_description == DESC_ATTACK2)
		{
			SOUNDMANAGER->Play3D("hydra_breath_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_BREATH_FIRE3:
	{
		if (pData->_description == DESC_ATTACK3)
		{
			SOUNDMANAGER->Play3D("hydra_breath_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_DEATH:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("hydra_death_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("lizard_fall_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_HIT1 :
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("hydra_hit_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_IDLE:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("hydra_idle_01", *pData->_pPosition);
		}
	}break;
	//case HYDRA_STAND:
	//{
	//}break;
	case HYDRA_RUN:
	{
		if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("hydra_walk_right", *pData->_pPosition);
		}
		else if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("hydra_walk_left", *pData->_pPosition);
		}
	}break;
	case HYDRA_WHIP_HEAD:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("hydra_whip_01", *pData->_pPosition);
		}
	}break;
	case HYDRA_WHIP_TAIL:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("hydra_whip_01", *pData->_pPosition);
		}
	}break;
	}
}
