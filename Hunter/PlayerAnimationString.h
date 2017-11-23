#ifndef PLAYER_ANIMATION_STRING_H
#define PLAYER_ANIMATION_STRING_H


#define PLAYER_ANIM_DECLARE(Name) \
	template <> \
	struct PlayerAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define PLAYER_ANIM_LINK(Name) \
	Action PlayerAnimationAction<Name>::_action;

#define PLAYER_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	PlayerAnimationAction<Name>::_action._blocking = Blocking; \
	PlayerAnimationAction<Name>::_action._playOnce = PlayOnce; \
	PlayerAnimationAction<Name>::_action._stop = Stop; \
	PlayerAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	PlayerAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	PlayerAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(PlayerAnimationAction<Name>::_action._name, PlayerAnimationString[PlayerAnimationEnum::Name], strlen(PlayerAnimationString[PlayerAnimationEnum::Name])); \
	PlayerAnimationAction<Name>::_action._name; 

#define PLAYER_ANIM( Name ) PlayerAnimationAction<Name>::_action

void InitPlayerAnimation();

enum PlayerAnimationEnum
{
	eWarRetreat = 0,
	eWalkingBack,
	eClimbingLadder,
	eCrouching,
	eDrinking,
	eEating,
	eExplanation,
	eJumpRunning,
	eRun,
	eSayNo,
	eSneaking,
	eStandingFree,
	eSwimming,
	eTalking,
	eWarThrowingAxe,
	eWarThrowingSpear,
	eWarCombatMode,
	eWarDying,
	eWarBackwards,
	eWarBelowHighSwing,
	eWarCharging,
	eWarDodgeToLeft,
	eWarDodgeToRight,
	eWarMovingLeft,
	eWarMovingRight,
	eWarParryFromStraightDown,
	eWarParryFront,
	eWarRunSwingLeft,
	eWarRunSwingRight,
	eWarRunSwingUpDown,
	eWarShieldBlock,
	eWarShieldBlow,
	eWarSpecialAttackA,
	eWarSpecialAttackB,
	eWarSwingHighStraigtDown,
	eWarSwingLeft,
	eWarSwingRight,
	eWarTakingHit,
	eWarThrustMid,
	eBoring,
	eCameUp,
	eLookingAround,
	eMakingItem,
	ePickObject,
	eSalute,
	eSitDown,
	eSitting,
	eWalk,
	COUNT
};

template <int32 Anim>
struct PlayerAnimationAction
{
	//static const Action _action;
};

static const char *PlayerAnimationString[] = 
{
	{"Hero_War_retreat"},
	{ "Hero_walking_back"},
	{ "hero_Climbing_ladder"},
	{ "hero_Crouching"},
	{ "hero_Drinking"},
	{ "hero_Eating"},
	{ "hero_Explanation"},
	{ "hero_Jump_runing"},
	{ "hero_Run"},
	{"hero_Say_No"},
	{"hero_Sneaking"},
	{"hero_Standing_Free"},
	{"hero_Swimming"},
	{"hero_Talking"},
	{"hero_ThrowingAxe_throwing"},
	{"hero_ThrowingSpear_throwing"},
	{"hero_War_COMBAT_MODE"},
	{"hero_War_Dying"},
	{"hero_War_backwards"},
	{"hero_War_below_high_swing"},
	{"hero_War_charging"},
	{"hero_War_dodge_to_left"},
	{"hero_War_dodge_to_right"},
	{"hero_War_moving_left"},
	{"hero_War_moving_right"},
	{"hero_War_parry_from_stright_dow"},
	{"hero_War_parry_front"},
	{"hero_War_run_swing_left"},
	{"hero_War_run_swing_right"},
	{"hero_War_run_swing_up_down"},
	{"hero_War_shield_block"},
	{"hero_War_shield_blow"},
	{"hero_War_special_attack_A"},
	{"hero_War_special_attack_B"},
	{"hero_war_swing_high_straight_do"},
	{"hero_War_swing_left"},
	{"hero_War_swing_right"},
	{"hero_War_taking_hit"},
	{"hero_War_thrust_mid"},
	{"hero_boring"},
	{"hero_came_up"},
	{"hero_looking_around"},
	{"hero_making_item"},
	{"hero_pick_object"},
	{"hero_salute"},
	{"hero_sit_down"},
	{"hero_sitting"},
	{"hero_walk"}
};

PLAYER_ANIM_DECLARE(eWarRetreat);
PLAYER_ANIM_DECLARE(eWalkingBack);
PLAYER_ANIM_DECLARE(eClimbingLadder);
PLAYER_ANIM_DECLARE(eCrouching);
PLAYER_ANIM_DECLARE(eDrinking);
PLAYER_ANIM_DECLARE(eEating);
PLAYER_ANIM_DECLARE(eExplanation);
PLAYER_ANIM_DECLARE(eJumpRunning);
PLAYER_ANIM_DECLARE(eRun);
PLAYER_ANIM_DECLARE(eSayNo);
PLAYER_ANIM_DECLARE(eSneaking);
PLAYER_ANIM_DECLARE(eStandingFree);
PLAYER_ANIM_DECLARE(eSwimming);
PLAYER_ANIM_DECLARE(eTalking);
PLAYER_ANIM_DECLARE(eWarThrowingAxe);
PLAYER_ANIM_DECLARE(eWarThrowingSpear);
PLAYER_ANIM_DECLARE(eWarCombatMode);
PLAYER_ANIM_DECLARE(eWarDying);
PLAYER_ANIM_DECLARE(eWarBackwards);
PLAYER_ANIM_DECLARE(eWarBelowHighSwing);
PLAYER_ANIM_DECLARE(eWarCharging);
PLAYER_ANIM_DECLARE(eWarDodgeToLeft);
PLAYER_ANIM_DECLARE(eWarDodgeToRight);
PLAYER_ANIM_DECLARE(eWarMovingLeft);
PLAYER_ANIM_DECLARE(eWarMovingRight);
PLAYER_ANIM_DECLARE(eWarParryFromStraightDown);
PLAYER_ANIM_DECLARE(eWarParryFront);
PLAYER_ANIM_DECLARE(eWarRunSwingLeft);
PLAYER_ANIM_DECLARE(eWarRunSwingRight);
PLAYER_ANIM_DECLARE(eWarRunSwingUpDown);
PLAYER_ANIM_DECLARE(eWarShieldBlock);
PLAYER_ANIM_DECLARE(eWarShieldBlow);
PLAYER_ANIM_DECLARE(eWarSpecialAttackA);
PLAYER_ANIM_DECLARE(eWarSpecialAttackB);
PLAYER_ANIM_DECLARE(eWarSwingHighStraigtDown);
PLAYER_ANIM_DECLARE(eWarSwingLeft);
PLAYER_ANIM_DECLARE(eWarSwingRight);
PLAYER_ANIM_DECLARE(eWarTakingHit);
PLAYER_ANIM_DECLARE(eWarThrustMid);
PLAYER_ANIM_DECLARE(eBoring);
PLAYER_ANIM_DECLARE(eCameUp);
PLAYER_ANIM_DECLARE(eLookingAround);
PLAYER_ANIM_DECLARE(eMakingItem);
PLAYER_ANIM_DECLARE(ePickObject);
PLAYER_ANIM_DECLARE(eSalute);
PLAYER_ANIM_DECLARE(eSitDown);
PLAYER_ANIM_DECLARE(eSitting);
PLAYER_ANIM_DECLARE(eWalk);

#endif
