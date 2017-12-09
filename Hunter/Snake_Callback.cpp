#include "stdafx.h"
#include "Snake.h"

#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Snake::SetupCallbackAndCompression()
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

#pragma region Attack
	//Attack1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_ATTACK1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.26) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Attack2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_ATTACK2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.36) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Attack3
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_ATTACK3], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.25) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK3];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Move
	//Move
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_MOVE], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.1) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Hit
	//Hit
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.5) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Dead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.5) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}

#pragma endregion

#pragma region Roar
	//Roar
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_ROAR], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.20) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

}

HRESULT SnakeCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	SnakeCallbackData* pData = (SnakeCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case SNAKE_ATTACK1 :
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case SNAKE_ATTACK2 :
	{
		if (pData->_description == DESC_ATTACK2)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case SNAKE_ATTACK3 :
	{
		if (pData->_description == DESC_ATTACK3)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case SNAKE_DEATH :
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case SNAKE_HIT1:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	//case SNAKE_IDLE:
	//{
	//}break;
	case SNAKE_MOVE:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	case SNAKE_ROAR:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("", *pData->_pPosition);
		}
	}break;
	//case SNAKE_STAND :
	//{
	//}break;
	}
}
