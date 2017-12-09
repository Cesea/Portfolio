#include "stdafx.h"
#include "Turtle.h"

#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Turtle::SetupCallbackAndCompression()
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
	//BITE1
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_BITE1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.40) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//BITE2
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_BITE2], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.34) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region HIT
	//Hit
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_HIT1], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.06) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
	//Dead
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_DEATH], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.10) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region ROAR
	//Roar
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_ROAR], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys;
		keys.Time = (anim->GetPeriod() * 0.28) * anim->GetSourceTicksPerSecond();
		keys.pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

#pragma region Walk
	//Walk
	{
		ID3DXKeyframedAnimationSet *anim;
		pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_WALK], (ID3DXAnimationSet **)&anim);

		D3DXKEY_CALLBACK keys[2];
		keys[0].Time = (anim->GetPeriod() * 0.05) * anim->GetSourceTicksPerSecond();
		keys[0].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

		keys[1].Time = (anim->GetPeriod() * 0.48) * anim->GetSourceTicksPerSecond();
		keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

		AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
	}
#pragma endregion

}

HRESULT TurtleCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	TurtleCallbackData* pData = (TurtleCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}

	switch (*pData->_animtionEnum)
	{
	case TURTLE_BITE1:
	{
		if (pData->_description == DESC_ATTACK1)
		{
			SOUNDMANAGER->Play3D("turtle_attack_01", *pData->_pPosition);
		}
	}break;
	case TURTLE_BITE2:
	{
		if (pData->_description == DESC_ATTACK2)
		{
			SOUNDMANAGER->Play3D("turtle_attack_01", *pData->_pPosition);
		}
	}break;
	case TURTLE_DEATH:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("turtle_dead_01", *pData->_pPosition);
		}
	}break;
	case TURTLE_HIT1:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("turtle_hit_01", *pData->_pPosition);
		}
	}break;
	case TURTLE_ROAR:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("turtle_roar_01", *pData->_pPosition);
		}
	}break;
	case TURTLE_WALK:
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("turtle_walk_left", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("turtle_walk_right", *pData->_pPosition);
		}
	}break;
	}

}
