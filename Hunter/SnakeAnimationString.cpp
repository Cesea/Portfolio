#include "stdafx.h"
#include "SnakeAnimationString.h"

SNAKE_ANIM_LINK(SNAKE_ATTACK1);
SNAKE_ANIM_LINK(SNAKE_ATTACK2);
SNAKE_ANIM_LINK(SNAKE_ATTACK3);
SNAKE_ANIM_LINK(SNAKE_DEATH);
SNAKE_ANIM_LINK(SNAKE_HIT1);
SNAKE_ANIM_LINK(SNAKE_HIT2);
SNAKE_ANIM_LINK(SNAKE_IDLE);
SNAKE_ANIM_LINK(SNAKE_MOVE);
SNAKE_ANIM_LINK(SNAKE_ROAR);
SNAKE_ANIM_LINK(SNAKE_STAND);

void InitSnakeAnimation()
{
	SNAKE_ANIM_DEFINE(SNAKE_ATTACK1, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_ATTACK2, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_ATTACK3, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_DEATH, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_HIT1, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_HIT2, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_IDLE, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_MOVE, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_ROAR, false, false, false, 1.0f, 0.2f, 0.2f);
	SNAKE_ANIM_DEFINE(SNAKE_STAND, false, false, false, 1.0f, 0.2f, 0.2f);
}
