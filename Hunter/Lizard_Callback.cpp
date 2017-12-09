#include "stdafx.h"
#include "Lizard.h"

#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Lizard::SetupCallbackAndCompression()
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

	ID3DXAnimationController *pController = _pActionComp->_pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();


#pragma region Walk
	//Walk
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_WALK], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.06) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

		keys[1].Time = (anim->GetPeriod() * 0.40) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Run
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_RUN], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		keys[1].Time = (anim->GetPeriod() * 0.47) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Attack
	//Attack
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_ATTACK], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.40) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Bite
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_BITE], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.44) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Spit
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_SPIT], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Hit
	//Hit
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.07) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Dead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.15) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Roar
	//Roar
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_ROAR], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.15) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

}


HRESULT LizardCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	LizardCallbackData* pData = (LizardCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case LIZARD_ATTACK:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_BITE:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_DEATH:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_HIT1:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_HIT2:
	{
	}break;
	case LIZARD_IDLE:
	{
	}break;
	case LIZARD_ROAR:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_RUN:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_SPIT:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case LIZARD_STAND:
	{
	}break;
	case LIZARD_WALK:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	}
	return S_OK;
}
