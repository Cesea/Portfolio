#include "stdafx.h"
#include "CatAnimationString.h"

CAT_ANIM_LINK(CAT_ATTACK1);
CAT_ANIM_LINK(CAT_ATTACK2);
CAT_ANIM_LINK(CAT_ATTACK3);
CAT_ANIM_LINK(CAT_ATTACK4);
CAT_ANIM_LINK(CAT_ATTACK5);
CAT_ANIM_LINK(CAT_DEATH);
CAT_ANIM_LINK(CAT_HIT1);
CAT_ANIM_LINK(CAT_HIT2);
CAT_ANIM_LINK(CAT_IDLE);
CAT_ANIM_LINK(CAT_RUN);
CAT_ANIM_LINK(CAT_STAND);
CAT_ANIM_LINK(CAT_WALK);

void InitCatAnimation()
{
	CAT_ANIM_DEFINE(CAT_ATTACK1, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_ATTACK2, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_ATTACK3, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_ATTACK4, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_ATTACK5, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_DEATH, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_HIT1, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_HIT2, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_IDLE, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_RUN, false, false, false, 1.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_STAND, false, true, false, 2.0f, 0.2f, 0.2f);
	CAT_ANIM_DEFINE(CAT_WALK, false, false, false, 1.0f, 0.2f, 0.2f);
}
