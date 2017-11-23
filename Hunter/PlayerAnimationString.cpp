#include "stdafx.h"
#include "PlayerAnimationString.h"

#include "ActionComponent.h"

//PLAYER_ANIM_DEFINE(eSwimming, false, false, 1.0f, 0.2f, 0.2f)

PLAYER_ANIM_LINK(eWarRetreat);
PLAYER_ANIM_LINK(eWalkingBack);
PLAYER_ANIM_LINK(eClimbingLadder);
PLAYER_ANIM_LINK(eCrouching);
PLAYER_ANIM_LINK(eDrinking);
PLAYER_ANIM_LINK(eEating);
PLAYER_ANIM_LINK(eExplanation);
PLAYER_ANIM_LINK(eJumpRunning);
PLAYER_ANIM_LINK(eRun);
PLAYER_ANIM_LINK(eSayNo);
PLAYER_ANIM_LINK(eSneaking);
PLAYER_ANIM_LINK(eStandingFree);
PLAYER_ANIM_LINK(eSwimming);
PLAYER_ANIM_LINK(eTalking);
PLAYER_ANIM_LINK(eWarThrowingAxe);
PLAYER_ANIM_LINK(eWarThrowingSpear);
PLAYER_ANIM_LINK(eWarCombatMode);
PLAYER_ANIM_LINK(eWarDying);
PLAYER_ANIM_LINK(eWarBackwards);
PLAYER_ANIM_LINK(eWarBelowHighSwing);
PLAYER_ANIM_LINK(eWarCharging);
PLAYER_ANIM_LINK(eWarDodgeToLeft);
PLAYER_ANIM_LINK(eWarDodgeToRight);
PLAYER_ANIM_LINK(eWarMovingLeft);
PLAYER_ANIM_LINK(eWarMovingRight);
PLAYER_ANIM_LINK(eWarParryFromStraightDown);
PLAYER_ANIM_LINK(eWarParryFront);
PLAYER_ANIM_LINK(eWarRunSwingLeft);
PLAYER_ANIM_LINK(eWarRunSwingRight);
PLAYER_ANIM_LINK(eWarRunSwingUpDown);
PLAYER_ANIM_LINK(eWarShieldBlock);
PLAYER_ANIM_LINK(eWarShieldBlow);
PLAYER_ANIM_LINK(eWarSpecialAttackA);
PLAYER_ANIM_LINK(eWarSpecialAttackB);
PLAYER_ANIM_LINK(eWarSwingHighStraigtDown);
PLAYER_ANIM_LINK(eWarSwingLeft);
PLAYER_ANIM_LINK(eWarSwingRight);
PLAYER_ANIM_LINK(eWarTakingHit);
PLAYER_ANIM_LINK(eWarThrustMid);
PLAYER_ANIM_LINK(eBoring);
PLAYER_ANIM_LINK(eCameUp);
PLAYER_ANIM_LINK(eLookingAround);
PLAYER_ANIM_LINK(eMakingItem);
PLAYER_ANIM_LINK(ePickObject);
PLAYER_ANIM_LINK(eSalute);
PLAYER_ANIM_LINK(eSitDown);
PLAYER_ANIM_LINK(eSitting);
PLAYER_ANIM_LINK(eWalk);


void InitPlayerAnimation()
{
	PLAYER_ANIM_DEFINE(eWarRetreat, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWalkingBack, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eClimbingLadder, true, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eCrouching, false, false, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eDrinking, true, true, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eEating, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eExplanation, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eJumpRunning, true, true, false, 1.0f, 0.15f, 0.2f);
	PLAYER_ANIM_DEFINE(eRun, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSayNo, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSneaking, true, false, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eStandingFree, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSwimming, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eTalking, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarThrowingAxe, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarThrowingSpear, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarCombatMode, false, false, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarDying, false, false, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarBackwards, false, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarBelowHighSwing, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarCharging, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarDodgeToLeft, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarDodgeToRight, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarMovingLeft, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarMovingRight, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarParryFromStraightDown, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarParryFront, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarRunSwingLeft, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarRunSwingRight, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarRunSwingUpDown, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarShieldBlock, true, true, false, 1.0f, 0.2f, 0.2f); //??
	PLAYER_ANIM_DEFINE(eWarShieldBlow, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarSpecialAttackA, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarSpecialAttackB, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarSwingHighStraigtDown, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarSwingLeft, true, true, false, 1.0f, 0.2f, 0.15f);
	PLAYER_ANIM_DEFINE(eWarSwingRight, true, true, false, 1.0f, 0.2f, 0.15f);
	PLAYER_ANIM_DEFINE(eWarTakingHit, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWarThrustMid, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eBoring, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eCameUp, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eLookingAround, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eMakingItem, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(ePickObject, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSalute, true, true, false, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSitDown, true, false, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eSitting, true, false, true, 1.0f, 0.2f, 0.2f);
	PLAYER_ANIM_DEFINE(eWalk, false, false, false, 1.0f, 0.2f, 0.2f);
}
