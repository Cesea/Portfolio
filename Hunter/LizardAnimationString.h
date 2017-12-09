#ifndef LIZARD_ANIMATION_STRING_H
#define LIZARD_ANIMATION_STRING_H

#define LIZARD_ANIM_DECLARE(Name) \
	template <> \
	struct LizardAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define LIZARD_ANIM_LINK(Name) \
	Action LizardAnimationAction<Name>::_action;

#define LIZARD_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	LizardAnimationAction<Name>::_action._blocking = Blocking; \
	LizardAnimationAction<Name>::_action._playOnce = PlayOnce; \
	LizardAnimationAction<Name>::_action._stop = Stop; \
	LizardAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	LizardAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	LizardAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(LizardAnimationAction<Name>::_action._name, LizardAnimationString[LIZARD_ANIMATION_ENUM::Name], strlen(LizardAnimationString[LIZARD_ANIMATION_ENUM::Name])); \
	LizardAnimationAction<Name>::_action._name; \
	LizardAnimationAction<Name>::_action._enum = Name;; 

#define LIZARD_ANIM( Name ) LizardAnimationAction<Name>::_action

void InitLizardAnimation();

template <int32 Anim>
struct LizardAnimationAction
{
	//static const Action _action;
};

enum LIZARD_ANIMATION_ENUM
{
	LIZARD_ATTACK,
	LIZARD_BITE,
	LIZARD_DEATH,
	LIZARD_HIT1,
	LIZARD_HIT2,
	LIZARD_IDLE,
	LIZARD_ROAR,
	LIZARD_RUN,
	LIZARD_SPIT,
	LIZARD_STAND,
	LIZARD_WALK
};

static const char *LizardAnimationString[] =
{
	{ "attack" },
	{ "bite" },
	{ "death" },
	{ "hit1" },
	{ "hit2" },
	{ "idle" },
	{ "roar" },
	{"run"},
	{"spit"},
	{"stand"},
	{"walk"}
};

LIZARD_ANIM_DECLARE(LIZARD_ATTACK)
LIZARD_ANIM_DECLARE(LIZARD_BITE)
LIZARD_ANIM_DECLARE(LIZARD_DEATH)
LIZARD_ANIM_DECLARE(LIZARD_HIT1)
LIZARD_ANIM_DECLARE(LIZARD_HIT2)
LIZARD_ANIM_DECLARE(LIZARD_IDLE)
LIZARD_ANIM_DECLARE(LIZARD_ROAR)
LIZARD_ANIM_DECLARE(LIZARD_RUN)
LIZARD_ANIM_DECLARE(LIZARD_SPIT)
LIZARD_ANIM_DECLARE(LIZARD_STAND)
LIZARD_ANIM_DECLARE(LIZARD_WALK)

#endif
