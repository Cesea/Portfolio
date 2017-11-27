#ifndef BAT_ANIMATION_STRING_H
#define BAT_ANIMATION_STRING_H

#define BAT_ANIM_DECLARE(Name) \
	template <> \
	struct BatAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define BAT_ANIM_LINK(Name) \
	Action BatAnimationAction<Name>::_action;

#define BAT_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	BatAnimationAction<Name>::_action._blocking = Blocking; \
	BatAnimationAction<Name>::_action._playOnce = PlayOnce; \
	BatAnimationAction<Name>::_action._stop = Stop; \
	BatAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	BatAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	BatAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(BatAnimationAction<Name>::_action._name, BatAnimationString[BAT_ANIMATION_ENUM::Name], strlen(BatAnimationString[BAT_ANIMATION_ENUM::Name])); \
	BatAnimationAction<Name>::_action._name; 

#define BAT_ANIM( Name ) BatAnimationAction<Name>::_action

void InitBatAnimation();

template <int32 Anim>
struct BatAnimationAction
{
	//static const Action _action;
};

enum BAT_ANIMATION_ENUM
{
	BAT_ATTACK1,
	BAT_ATTACK2,
	BAT_ATTACK3,
	BAT_DEATH,
	BAT_FORWARD,
	BAT_IDLE,
	BAT_HIT1,
	BAT_HIT2,
	BAT_ROAR,
};

static const char *BatAnimationString[] =
{
	{ "attack1" },
	{ "attack2" },
	{ "attack3" },
	{ "death" },
	{ "fly_forward" },
	{ "fly_in_place" },
	{ "hit1" },
	{ "hit2" },
	{ "roar" },
};

BAT_ANIM_DECLARE(BAT_ATTACK1);
BAT_ANIM_DECLARE(BAT_ATTACK2);
BAT_ANIM_DECLARE(BAT_ATTACK3);
BAT_ANIM_DECLARE(BAT_DEATH);
BAT_ANIM_DECLARE(BAT_FORWARD);
BAT_ANIM_DECLARE(BAT_IDLE);
BAT_ANIM_DECLARE(BAT_HIT1);
BAT_ANIM_DECLARE(BAT_HIT2);
BAT_ANIM_DECLARE(BAT_ROAR);

#endif
