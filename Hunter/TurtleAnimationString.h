#ifndef TURTLE_ANIMATION_STRING_H
#define TURTLE_ANIMATION_STRING_H

#define TURTLE_ANIM_DECLARE(Name) \
	template <> \
	struct TurtleAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define TURTLE_ANIM_LINK(Name) \
	Action TurtleAnimationAction<Name>::_action;

#define TURTLE_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	TurtleAnimationAction<Name>::_action._blocking = Blocking; \
	TurtleAnimationAction<Name>::_action._playOnce = PlayOnce; \
	TurtleAnimationAction<Name>::_action._stop = Stop; \
	TurtleAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	TurtleAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	TurtleAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(TurtleAnimationAction<Name>::_action._name, TurtleAnimationString[TURTLE_ANIMATION_ENUM::Name], strlen(TurtleAnimationString[TURTLE_ANIMATION_ENUM::Name])); \
	TurtleAnimationAction<Name>::_action._name; 

#define TURTLE_ANIM( Name ) TurtleAnimationAction<Name>::_action

void InitTurtleAnimation();

template <int32 Anim>
struct TurtleAnimationAction
{
	//static const Action _action;
};

enum TURTLE_ANIMATION_ENUM
{
	TURTLE_ATTACK1,
	TURTLE_ATTACK2,
	TURTLE_ATTACK3,
	TURTLE_DEATH,
	TURTLE_HIT1,
	TURTLE_HIT2,
	TURTLE_IDLE,
	TURTLE_MOVE,
	TURTLE_ROAR,
	TURTLE_STAND,
};

static const char *TurtleAnimationString[] =
{
	{ "attack1" },
	{ "attack2" },
	{ "attack3" },
	{ "death" },
	{ "hit1" },
	{ "hit2" },
	{ "idle" },
	{ "move" },
	{ "roar" },
	{ "stand" },
};

TURTLE_ANIM_DECLARE(TURTLE_ATTACK1);
TURTLE_ANIM_DECLARE(TURTLE_ATTACK2);
TURTLE_ANIM_DECLARE(TURTLE_ATTACK3);
TURTLE_ANIM_DECLARE(TURTLE_DEATH);
TURTLE_ANIM_DECLARE(TURTLE_HIT1);
TURTLE_ANIM_DECLARE(TURTLE_HIT2);
TURTLE_ANIM_DECLARE(TURTLE_IDLE);
TURTLE_ANIM_DECLARE(TURTLE_MOVE);
TURTLE_ANIM_DECLARE(TURTLE_ROAR);
TURTLE_ANIM_DECLARE(TURTLE_STAND);

#endif
