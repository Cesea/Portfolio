#include "stdafx.h"
#include "Player.h"

void Player::SetupCallbackAndCompression()
{
   _callbackData._animtionEnum = (PlayerAnimationEnum *)&_animationEnum;

   ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
   TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

   ID3DXAnimationController *pController = refActionComp._pAnimationController;
   uint32 numAnimationSet = pController->GetNumAnimationSets();

   //War Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time =anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Swing Right
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time = anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Thrust Mid
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarThrustMid], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time = anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Walk Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkSwingLeft], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time =anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Walk Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkSwingRight], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time =anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Walk WalkThrust Mid
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkThrust], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time =anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }


   //War Block
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarShieldBlock], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time = anim->GetPeriod() / 1.5f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }
}

HRESULT PlayerCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	PlayerCallbackData* pData = (PlayerCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}
	switch (*pData->_animtionEnum)
	{
	case PlayerAnimationEnum::eWarSwingLeft:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;
	case PlayerAnimationEnum::eWarSwingRight:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;
	case PlayerAnimationEnum::eWarThrustMid :
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;
	case PlayerAnimationEnum::eWarWalkSwingLeft:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;
	case PlayerAnimationEnum::eWarWalkSwingRight:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;
	case PlayerAnimationEnum::eWarWalkThrust:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;
	}break;

	//Shield
	case PlayerAnimationEnum::eWarShieldBlock :
	{
		_pPlayer->_canCombo = true;
	}

	//case PlayerAnimationEnum::eWar:
	//{
	//	//_pPlayer->_canCombo = true;
	//}break;
	//case PlayerAnimationEnum::eWarSwingLeft:
	//{
	//	//_pPlayer->_canCombo = true;
	//}break;
	//case PlayerAnimationEnum::eWarSwingLeft:
	//{
	//	//_pPlayer->_canCombo = true;
	//}break;
	//case PlayerAnimationEnum::eWarSwingLeft:
	//{
	//	//_pPlayer->_canCombo = true;
	//}break;
	//case PlayerAnimationEnum::eWarSwingLeft:
	//{
	//	//_pPlayer->_canCombo = true;
	//}break;
	}
	return S_OK;
}
