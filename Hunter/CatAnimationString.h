#ifndef CAT_ANIMATION_STRING_H
#define CAT_ANIMATION_STRING_H

#define CAT_ANIM_DECLARE(Name) \
	template <> \
	struct CatAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define CAT_ANIM_LINK(Name) \
	Action CatAnimationAction<Name>::_action;

#define CAT_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	CatAnimationAction<Name>::_action._blocking = Blocking; \
	CatAnimationAction<Name>::_action._playOnce = PlayOnce; \
	CatAnimationAction<Name>::_action._stop = Stop; \
	CatAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	CatAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	CatAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(CatAnimationAction<Name>::_action._name, CatAnimationString[CAT_ANIMATION_ENUM::Name], strlen(CatAnimationString[CAT_ANIMATION_ENUM::Name])); \
	CatAnimationAction<Name>::_action._name; 

#define CAT_ANIM( Name ) CatAnimationAction<Name>::_action

void InitCatAnimation();

template <int32 Anim>
struct CatAnimationAction
{
	//static const Action _action;
};

enum CAT_ANIMATION_ENUM
{
	CAT_ATTACK1,
	CAT_ATTACK2,
	CAT_ATTACK3,
	CAT_ATTACK4,
	CAT_ATTACK5,
	CAT_DEATH,
	CAT_HIT1,
	CAT_HIT2,
	CAT_IDLE,
	CAT_RUN,
	CAT_STAND,
	CAT_WALK
};

static const char *CatAnimationString[] =
{
	{ "attack1" },
	{ "attack2" },
	{ "attack3" },
	{ "attack4" },
	{ "attack5" },
	{ "death" },
	{ "hit1" },
	{ "hit2" },
	{ "idle" },
	{ "run" },
	{ "stand" },
	{ "walk" },
};

CAT_ANIM_DECLARE(CAT_ATTACK1);
CAT_ANIM_DECLARE(CAT_ATTACK2);
CAT_ANIM_DECLARE(CAT_ATTACK3);
CAT_ANIM_DECLARE(CAT_ATTACK4);
CAT_ANIM_DECLARE(CAT_ATTACK5);
CAT_ANIM_DECLARE(CAT_DEATH);
CAT_ANIM_DECLARE(CAT_HIT1);
CAT_ANIM_DECLARE(CAT_HIT2);
CAT_ANIM_DECLARE(CAT_IDLE);
CAT_ANIM_DECLARE(CAT_RUN);
CAT_ANIM_DECLARE(CAT_STAND);
CAT_ANIM_DECLARE(CAT_WALK);

#endif

