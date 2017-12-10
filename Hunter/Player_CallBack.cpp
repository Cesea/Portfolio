#include "stdafx.h"
#include "Player.h"

//NOTE : Callback Description
#define DESC_LEFT (0)
#define DESC_RIGHT (1)
#define DESC_ATTACK1 (2)
#define DESC_ATTACK2 (3)
#define DESC_ATTACK3 (4)
#define DESC_ATTACK4 (5)

void Player::SetupCallbackAndCompression()
{
	_callbackData[0]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[0]._pPosition = &_pTransformComp->_position;
	_callbackData[0]._description = DESC_LEFT;

	_callbackData[1]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[1]._pPosition = &_pTransformComp->_position;
	_callbackData[1]._description = DESC_RIGHT;

	_callbackData[2]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[2]._pPosition = &_pTransformComp->_position;
	_callbackData[2]._description = DESC_ATTACK1;

	_callbackData[3]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[3]._pPosition = &_pTransformComp->_position;
	_callbackData[3]._description = DESC_ATTACK2;

	_callbackData[4]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[4]._pPosition = &_pTransformComp->_position;
	_callbackData[4]._description = DESC_ATTACK3;

	_callbackData[5]._animtionEnum = &_pActionComp->_animationEnum;
	_callbackData[5]._pPosition = &_pTransformComp->_position;
	_callbackData[5]._description = DESC_ATTACK4;

   ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
   TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

   ID3DXAnimationController *pController = refActionComp._pAnimationController;
   uint32 numAnimationSet = pController->GetNumAnimationSets();



#pragma region Walk
   //Walk
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWalk], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() * 0.08f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() * 0.56f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0f);
   }

   //WalkBack
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWalkingBack], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() / 9.92f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() / 2.02f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Retreat
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarRetreat], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() / 9.92f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() / 2.02f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   }


   //Strafe Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eStrafeLeft], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() * 0.55f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[1];

	   keys[1].Time = (anim->GetPeriod() * 0.9f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[0];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0);
   }

   //Strafe Right
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eStrafeRight], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() / 2.02f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() / 1.02f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   }

   //War Moving Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarMovingLeft], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() / 2.02f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() / 1.02f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   }
   //War Moving Right
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarMovingRight], 
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() / 2.02f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() / 1.02f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   }


#pragma endregion

#pragma region Run

   //War Run
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarCharging],
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() * 0.3f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[0];

	   keys[1].Time = (anim->GetPeriod() * 0.8f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[1];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0f);
   }

#pragma endregion

#pragma region PlayerAttack

   //War Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys;
	   keys.Time = (anim->GetPeriod() * 0.33f) * anim->GetSourceTicksPerSecond();
	   keys.pCallbackData = (void *)&_callbackData[2];

	   AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.0);
   }

   //War Swing Right
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys;
	   keys.Time = (anim->GetPeriod() * 0.33f) * anim->GetSourceTicksPerSecond();
	   keys.pCallbackData = (void *)&_callbackData[3];

	   AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.3f);
   }

   //War Thrust Mid
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarThrustMid], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys;
	   keys.Time = (anim->GetPeriod() * 0.33f) * anim->GetSourceTicksPerSecond();
	   keys.pCallbackData = (void *)&_callbackData[4];

	   AddCallbackKeysAndCompress(pController, anim, 1, &keys, D3DXCOMPRESS_DEFAULT, 0.3f);
   }

   //War Walk Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkSwingLeft], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[3];
	   keys[0].Time = (anim->GetPeriod() * 0.39f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

	   keys[1].Time = (anim->GetPeriod() * 0.50f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

	   keys[2].Time = (anim->GetPeriod() * 0.92f) * anim->GetSourceTicksPerSecond();
	   keys[2].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

	   AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.3f);
   }

   //War Walk Swing Left
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkSwingRight], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[3];
	   keys[0].Time = (anim->GetPeriod() * 0.40f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[DESC_ATTACK2];

	   keys[1].Time = (anim->GetPeriod() * 0.50f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

	   keys[2].Time = (anim->GetPeriod() * 0.92f) * anim->GetSourceTicksPerSecond();
	   keys[2].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

	   AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.0f);
   }

   //War Walk WalkThrust Mid
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarWalkThrust], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[3];
	   keys[0].Time = (anim->GetPeriod() * 0.40f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[DESC_ATTACK3];

	   keys[1].Time = (anim->GetPeriod() * 0.50f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[DESC_LEFT];

	   keys[2].Time = (anim->GetPeriod() * 0.92f) * anim->GetSourceTicksPerSecond();
	   keys[2].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

	   AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.0f);
   }

#pragma endregion

   //War Block
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarShieldBlock], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[2];
	   keys[0].Time = (anim->GetPeriod() * 0.05f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

	   keys[1].Time = (anim->GetPeriod() * 0.4f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

	   AddCallbackKeysAndCompress(pController, anim, 2, keys, D3DXCOMPRESS_DEFAULT, 0.0f);
   }

   //Talking hit
   {
	   ID3DXKeyframedAnimationSet *anim;
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarTakingHit],
		   (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK keys[3];
	   keys[0].Time = (anim->GetPeriod() * 0.05f) * anim->GetSourceTicksPerSecond();
	   keys[0].pCallbackData = (void *)&_callbackData[DESC_LEFT];

	   keys[1].Time = (anim->GetPeriod() * 0.15f) * anim->GetSourceTicksPerSecond();
	   keys[1].pCallbackData = (void *)&_callbackData[DESC_RIGHT];

	   keys[2].Time = (anim->GetPeriod() * 0.9f) * anim->GetSourceTicksPerSecond();
	   keys[2].pCallbackData = (void *)&_callbackData[DESC_ATTACK1];

	   AddCallbackKeysAndCompress(pController, anim, 3, keys, D3DXCOMPRESS_DEFAULT, 0.1f);
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
	case PlayerAnimationEnum::eWalk :
	case PlayerAnimationEnum::eStrafeLeft :
	case PlayerAnimationEnum::eStrafeRight :
	case PlayerAnimationEnum::eWarMovingLeft :
	case PlayerAnimationEnum::eWarMovingRight :
	case PlayerAnimationEnum::eWalkingBack :
	case PlayerAnimationEnum::eWarRetreat :
	{
		if (pData->_description == 0)
		{
			SOUNDMANAGER->Play3D("player_walk_left", *pData->_pPosition);
		}
		else if(pData->_description == 1)
		{
			SOUNDMANAGER->Play3D("player_walk_right", *pData->_pPosition);
		}

	}break;

	case PlayerAnimationEnum::eWarCharging :
	{
		if (pData->_description == 0)
		{
			SOUNDMANAGER->Play3D("player_run_left", *pData->_pPosition);
		}
		else if(pData->_description == 1)
		{
			SOUNDMANAGER->Play3D("player_run_right", *pData->_pPosition);
		}
	}

	case PlayerAnimationEnum::eWarSwingLeft:
	case PlayerAnimationEnum::eWarWalkSwingLeft:
	{

		if (pData->_description == 0)
		{
			SOUNDMANAGER->Play3D("player_walk_left", *pData->_pPosition);
		}
		else if (pData->_description == 1)
		{
			SOUNDMANAGER->Play3D("player_walk_right", *pData->_pPosition);
		}
		else if (pData->_description == 2)
		{
			_pPlayer->_canCombo = true;
			_pPlayer->_pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid = true;
			SOUNDMANAGER->Play3D("player_swing_01", *pData->_pPosition);
		}

	}break;
	case PlayerAnimationEnum::eWarSwingRight:
	case PlayerAnimationEnum::eWarWalkSwingRight:
	{

		if (pData->_description == 0)
		{
			SOUNDMANAGER->Play3D("player_walk_left", *pData->_pPosition);
		}
		else if (pData->_description == 1)
		{
			SOUNDMANAGER->Play3D("player_walk_right", *pData->_pPosition);
		}
		if (pData->_description == 3)
		{
			_pPlayer->_canCombo = true;
			_pPlayer->_pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid = true;
			SOUNDMANAGER->Play3D("player_swing_02", *pData->_pPosition);
		}
	}break;
	case PlayerAnimationEnum::eWarThrustMid :
	case PlayerAnimationEnum::eWarWalkThrust:
	{

		if (pData->_description == 0)
		{
			SOUNDMANAGER->Play3D("player_walk_left", *pData->_pPosition);
		}
		else if (pData->_description == 1)
		{
			SOUNDMANAGER->Play3D("player_walk_right", *pData->_pPosition);
		}
		else if (pData->_description == 4)
		{
			_pPlayer->_canCombo = true;
			_pPlayer->_pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid = true;
			SOUNDMANAGER->Play3D("player_swing_03", *pData->_pPosition);
		}
	}break;

	case PlayerAnimationEnum::eWarTakingHit :
	{
		if (pData->_description == DESC_LEFT)
		{
			SOUNDMANAGER->Play3D("player_hit_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("player_hit_talk_01", *pData->_pPosition);
		}
		else if (pData->_description == DESC_ATTACK1)
		{
			_pPlayer->_state = Player::PLAYERSTATE_STANCE;
			_pPlayer->QueueAction(PLAYER_ANIM(eWarCombatMode));
		}
	}break;

	case PlayerAnimationEnum::eWarShieldBlock :
	{
		if (pData->_description == DESC_ATTACK1)
		{
			_pPlayer->_canCombo = true;
			SOUNDMANAGER->Play3D("player_shield_draw", *pData->_pPosition);
		}
		else if (pData->_description == DESC_RIGHT)
		{
			SOUNDMANAGER->Play3D("player_walk_right", *pData->_pPosition);
		}
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
