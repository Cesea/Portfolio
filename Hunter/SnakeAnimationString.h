#ifndef SNAKE_ANIMATION_STRING_H
#define SNAKE_ANIMATION_STRING_H

#define SNAKE_ANIM_DECLARE(Name) \
	template <> \
	struct SnakeAnimationAction<Name> \
	{ \
		static Action _action; \
	}; \

#define SNAKE_ANIM_LINK(Name) \
	Action SnakeAnimationAction<Name>::_action;

#define SNAKE_ANIM_DEFINE(Name, Blocking, PlayOnce, Stop, PlaySpeed, CrossFadeTime, OutCrossFadeTime)  \
	SnakeAnimationAction<Name>::_action._blocking = Blocking; \
	SnakeAnimationAction<Name>::_action._playOnce = PlayOnce; \
	SnakeAnimationAction<Name>::_action._stop = Stop; \
	SnakeAnimationAction<Name>::_action._playSpeed = PlaySpeed; \
	SnakeAnimationAction<Name>::_action._crossFadeTime = CrossFadeTime; \
	SnakeAnimationAction<Name>::_action._outCrossFadeTime = OutCrossFadeTime; \
	strncpy(SnakeAnimationAction<Name>::_action._name, SnakeAnimationString[SNAKE_ANIMATION_ENUM::Name], strlen(SnakeAnimationString[SNAKE_ANIMATION_ENUM::Name])); \
	SnakeAnimationAction<Name>::_action._name;  \
	SnakeAnimationAction<Name>::_action._enum = Name; 

#define SNAKE_ANIM( Name ) SnakeAnimationAction<Name>::_action

void InitSnakeAnimation();

template <int32 Anim>
struct SnakeAnimationAction
{
	//static const Action _action;
};

enum SNAKE_ANIMATION_ENUM
{
	SNAKE_ATTACK1,
	SNAKE_ATTACK2,
	SNAKE_ATTACK3,
	SNAKE_DEATH,
	SNAKE_HIT1,
	SNAKE_HIT2,
	SNAKE_IDLE,
	SNAKE_MOVE,
	SNAKE_ROAR,
	SNAKE_STAND,
};

static const char *SnakeAnimationString[] = 
{
	{ "attack1"},
	{ "attack2"},
	{ "attack3"},
	{ "death"},
	{ "hit1"},
	{ "hit2"},
	{ "idle"},
	{ "move"},
	{ "roar"},
	{ "stand"},
};

SNAKE_ANIM_DECLARE(SNAKE_ATTACK1);
SNAKE_ANIM_DECLARE(SNAKE_ATTACK2);
SNAKE_ANIM_DECLARE(SNAKE_ATTACK3);
SNAKE_ANIM_DECLARE(SNAKE_DEATH);
SNAKE_ANIM_DECLARE(SNAKE_HIT1);
SNAKE_ANIM_DECLARE(SNAKE_HIT2);
SNAKE_ANIM_DECLARE(SNAKE_IDLE);
SNAKE_ANIM_DECLARE(SNAKE_MOVE);
SNAKE_ANIM_DECLARE(SNAKE_ROAR);
SNAKE_ANIM_DECLARE(SNAKE_STAND);

#endif

