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

   //Talking hit
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarTakingHit],
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time = anim->GetPeriod() / 1.0f * anim->GetSourceTicksPerSecond();
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
	case PlayerAnimationEnum::eWarSwingRight:
	case PlayerAnimationEnum::eWarThrustMid :
	case PlayerAnimationEnum::eWarWalkSwingLeft:
	case PlayerAnimationEnum::eWarWalkSwingRight:
	case PlayerAnimationEnum::eWarWalkThrust:
	{
		_pPlayer->_canCombo = true;
		_pPlayer->_pCollisionComp->_isTrigger = true;

		_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(
			GameObjectFactory::DamageBoxEvent(_pPlayer->_pTransformComp->GetWorldPosition() - Vector3(-1, -1, -1),
				_pPlayer->_pTransformComp->GetWorldPosition() - Vector3(1, 1, 1),
			10.0f, CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX, 0.0f, 0.0f, 0.3f));
	}break;

	case PlayerAnimationEnum::eWarTakingHit :
	{
		_pPlayer->_state = Player::PLAYERSTATE_STANCE;
		_pPlayer->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarTakingHit));

	}break;

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
