#include "stdafx.h"
#include "batAnimationString.h"


BAT_ANIM_LINK(BAT_ATTACK1);
BAT_ANIM_LINK(BAT_ATTACK2);
BAT_ANIM_LINK(BAT_ATTACK3);
BAT_ANIM_LINK(BAT_DEATH);
BAT_ANIM_LINK(BAT_FORWARD);
BAT_ANIM_LINK(BAT_IDLE);
BAT_ANIM_LINK(BAT_HIT1);
BAT_ANIM_LINK(BAT_HIT2);
BAT_ANIM_LINK(BAT_ROAR);
void InitBatAnimation()
{
	BAT_ANIM_DEFINE(BAT_ATTACK1, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_ATTACK2, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_ATTACK3, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_DEATH, true, true, true, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_FORWARD, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_IDLE, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_HIT1, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_HIT2, false, false, false, 1.0f, 0.2f, 0.2f);
	BAT_ANIM_DEFINE(BAT_ROAR, false, false, false, 1.0f, 0.2f, 0.2f);
}
