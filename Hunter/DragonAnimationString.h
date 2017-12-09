#ifndef DRAGON_ANIMATION_STRING_H
#define DRAGON_ANIMATION_STRING_H

#define DRAGON_ANIM_DECLARE(Name) \
	template <> \
	struct DragonAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define DRAGON_ANIM_LINK(Name) \
	Action DragonAnimationAction<Name>::_action;

#define DRAGON_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	DragonAnimationAction<Name>::_action._blocking = Blocking; \
	DragonAnimationAction<Name>::_action._playOnce = PlayOnce; \
	DragonAnimationAction<Name>::_action._stop = Stop; \
	DragonAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	DragonAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	DragonAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(DragonAnimationAction<Name>::_action._name, DragonAnimationString[DRAGON_ANIMATION_ENUM::Name], strlen(DragonAnimationString[DRAGON_ANIMATION_ENUM::Name])); \
	DragonAnimationAction<Name>::_action._name; 

#define DRAGON_ANIM( Name ) DragonAnimationAction<Name>::_action

void InitDragonAnimation();

template <int32 Anim>
struct DragonAnimationAction
{
	//static const Action _action;
};

enum DRAGON_ANIMATION_ENUM
{
	DRAGON_ATTACK1,
	DRAGON_ATTACK2,
	DRAGON_DEATH,
	DRAGON_EAT,
	DRAGON_FLY,
	DRAGON_FLY_ATTACK,
	DRAGON_FLY_BREATH,
	DRAGON_FLY_DEATH,
	DRAGON_FLY_FAST,
	DRAGON_FLY_HIT,
	DRAGON_HIT1,
	DRAGON_HIT2,
	DRAGON_IDLE,
	DRAGON_RUN,
	DRAGON_WALK,
	DRAGON_STAND,
	DRAGON_WHIP_TAIL
};

static const char *DragonAnimationString[] =
{
	{ "attack1" },
	{ "attack2" },
	{ "death" },
	{"eat"},
	{"fly"},
	{"fly_attack"},
	{"fly_breath_fire"},
	{"fly_death"},
	{"fly_fast"},
	{"fly_hit"},
	{ "hit1" },
	{ "hit2" },
	{ "idle" },
	{ "run" },
	{ "walk" },
	{ "stand" },
	{"whip_tail"}
};

DRAGON_ANIM_DECLARE(DRAGON_ATTACK1);
DRAGON_ANIM_DECLARE(DRAGON_ATTACK2);
DRAGON_ANIM_DECLARE(DRAGON_DEATH);
DRAGON_ANIM_DECLARE(DRAGON_EAT);
DRAGON_ANIM_DECLARE(DRAGON_FLY);
DRAGON_ANIM_DECLARE(DRAGON_FLY_ATTACK);
DRAGON_ANIM_DECLARE(DRAGON_FLY_BREATH);
DRAGON_ANIM_DECLARE(DRAGON_FLY_DEATH);
DRAGON_ANIM_DECLARE(DRAGON_FLY_FAST);
DRAGON_ANIM_DECLARE(DRAGON_FLY_HIT);
DRAGON_ANIM_DECLARE(DRAGON_HIT1);
DRAGON_ANIM_DECLARE(DRAGON_HIT2);
DRAGON_ANIM_DECLARE(DRAGON_IDLE);
DRAGON_ANIM_DECLARE(DRAGON_RUN);
DRAGON_ANIM_DECLARE(DRAGON_WALK);
DRAGON_ANIM_DECLARE(DRAGON_STAND);
DRAGON_ANIM_DECLARE(DRAGON_WHIP_TAIL);

#endif

