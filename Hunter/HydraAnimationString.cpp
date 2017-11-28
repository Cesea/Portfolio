#include "stdafx.h"
#include "HydraAnimationString.h"

HYDRA_ANIM_LINK(HYDRA_BITE1);
HYDRA_ANIM_LINK(HYDRA_BITE2);
HYDRA_ANIM_LINK(HYDRA_BITE3);
HYDRA_ANIM_LINK(HYDRA_BREATH_FIRE1);
HYDRA_ANIM_LINK(HYDRA_BREATH_FIRE2);
HYDRA_ANIM_LINK(HYDRA_BREATH_FIRE3);
HYDRA_ANIM_LINK(HYDRA_DEATH);
HYDRA_ANIM_LINK(HYDRA_HIT1);
HYDRA_ANIM_LINK(HYDRA_HIT2);
HYDRA_ANIM_LINK(HYDRA_IDLE);
HYDRA_ANIM_LINK(HYDRA_STAND);
HYDRA_ANIM_LINK(HYDRA_RUN);
HYDRA_ANIM_LINK(HYDRA_WHIP_HEAD);
HYDRA_ANIM_LINK(HYDRA_WHIP_TAIL);

void InitHydraAnimation()
{
	HYDRA_ANIM_DEFINE(HYDRA_BITE1 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_BITE2 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_BITE3 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_BREATH_FIRE1 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_BREATH_FIRE2, false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_BREATH_FIRE3 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_DEATH , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_HIT1 , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_HIT2, false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_IDLE , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_STAND , false, true, false, 2.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_RUN , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_WHIP_HEAD , false, false, false, 1.0f, 0.2f, 0.2f);
	HYDRA_ANIM_DEFINE(HYDRA_WHIP_TAIL, false, false, false, 1.0f, 0.2f, 0.2f);
}