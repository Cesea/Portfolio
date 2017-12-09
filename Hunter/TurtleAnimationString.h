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
	TurtleAnimationAction<Name>::_action._name;  \
	TurtleAnimationAction<Name>::_action._enum = Name; 

#define TURTLE_ANIM( Name ) TurtleAnimationAction<Name>::_action

void InitTurtleAnimation();

template <int32 Anim>
struct TurtleAnimationAction
{
	//static const Action _action;
};

enum TURTLE_ANIMATION_ENUM
{
	TURTLE_BITE1,
	TURTLE_BITE2,
	TURTLE_BREATHFIRE,
	TURTLE_DEATH,
	TURTLE_EARTHQUAKE,
	TURTLE_HIT1,
	TURTLE_HIT2,
	TURTLE_ROAR,
	TURTLE_SPIT,
	TURTLE_STAND,
	TURTLE_WALK
};

static const char *TurtleAnimationString[] =
{
	{ "bite1" },
	{ "bite2" },
	{ "breath_fire" },
	{ "death" },
	{ "earth_quake"},
	{ "hit1" },
	{ "hit2" },
	{ "roar" },
	{ "spit" },
	{ "stand" },
	{ "walk" },
};

TURTLE_ANIM_DECLARE(TURTLE_BITE1);
TURTLE_ANIM_DECLARE(TURTLE_BITE2);
TURTLE_ANIM_DECLARE(TURTLE_BREATHFIRE);
TURTLE_ANIM_DECLARE(TURTLE_DEATH);
TURTLE_ANIM_DECLARE(TURTLE_EARTHQUAKE);
TURTLE_ANIM_DECLARE(TURTLE_HIT1);
TURTLE_ANIM_DECLARE(TURTLE_HIT2);
TURTLE_ANIM_DECLARE(TURTLE_ROAR);
TURTLE_ANIM_DECLARE(TURTLE_SPIT);
TURTLE_ANIM_DECLARE(TURTLE_STAND);
TURTLE_ANIM_DECLARE(TURTLE_WALK);

#endif
