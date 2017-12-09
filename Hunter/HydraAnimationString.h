#ifndef HYDRA_ANIMATION_STRING_H
#define HYDRA_ANIMATION_STRING_H

#define HYDRA_ANIM_DECLARE(Name) \
	template <> \
	struct HydraAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define HYDRA_ANIM_LINK(Name) \
	Action HydraAnimationAction<Name>::_action;

#define HYDRA_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	HydraAnimationAction<Name>::_action._blocking = Blocking; \
	HydraAnimationAction<Name>::_action._playOnce = PlayOnce; \
	HydraAnimationAction<Name>::_action._stop = Stop; \
	HydraAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	HydraAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	HydraAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(HydraAnimationAction<Name>::_action._name, HydraAnimationString[HYDRA_ANIMATION_ENUM::Name], strlen(HydraAnimationString[HYDRA_ANIMATION_ENUM::Name])); \
	HydraAnimationAction<Name>::_action._name;  \
	HydraAnimationAction<Name>::_action._enum = Name; 

#define HYDRA_ANIM( Name ) HydraAnimationAction<Name>::_action

void InitHydraAnimation();

template <int32 Anim>
struct HydraAnimationAction
{
	//static const Action _action;
};

enum HYDRA_ANIMATION_ENUM
{
	HYDRA_BITE1,
	HYDRA_BITE2,
	HYDRA_BITE3,
	HYDRA_BREATH_FIRE1,
	HYDRA_BREATH_FIRE2,
	HYDRA_BREATH_FIRE3,
	HYDRA_DEATH,
	HYDRA_HIT1,
	HYDRA_HIT2,
	HYDRA_IDLE,
	HYDRA_STAND,
	HYDRA_RUN,
	HYDRA_WHIP_HEAD,
	HYDRA_WHIP_TAIL
};

static const char *HydraAnimationString[] =
{
	{ "bite1" },
	{ "bite2" },
	{ "bite3" },
	{ "breath_fire1" },
	{ "breath_fire2" },
	{ "breath_fire3" },
	{ "death" },
	{ "hit1" },
	{ "hit2" },
	{ "idle" },
	{ "stand" },
	{ "walk" },
	{ "whip_head" },
	{ "whip_tail" }
};

HYDRA_ANIM_DECLARE(HYDRA_BITE1);
HYDRA_ANIM_DECLARE(HYDRA_BITE2);
HYDRA_ANIM_DECLARE(HYDRA_BITE3);
HYDRA_ANIM_DECLARE(HYDRA_BREATH_FIRE1);
HYDRA_ANIM_DECLARE(HYDRA_BREATH_FIRE2);
HYDRA_ANIM_DECLARE(HYDRA_BREATH_FIRE3);
HYDRA_ANIM_DECLARE(HYDRA_DEATH);
HYDRA_ANIM_DECLARE(HYDRA_HIT1);
HYDRA_ANIM_DECLARE(HYDRA_HIT2);
HYDRA_ANIM_DECLARE(HYDRA_IDLE);
HYDRA_ANIM_DECLARE(HYDRA_STAND);
HYDRA_ANIM_DECLARE(HYDRA_RUN);
HYDRA_ANIM_DECLARE(HYDRA_WHIP_HEAD);
HYDRA_ANIM_DECLARE(HYDRA_WHIP_TAIL);

#endif

