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
	   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight], (ID3DXAnimationSet **)&anim);

	   D3DXKEY_CALLBACK key;
	   key.Time =anim->GetPeriod() / 2.0f * anim->GetSourceTicksPerSecond();
	   key.pCallbackData = (void *)&_callbackData;

	   AddCallbackKeysAndCompress(pController, anim, 1, &key, D3DXCOMPRESS_DEFAULT, 0.1f);
   }
}
