#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "AnimationUtils.h"

#include "AnimationComponent.h"
#include "RenderComponent.h"

class AnimationSystem : public System<Requires<RenderComponent, AnimationComponent>>
{
public :
	AnimationSystem();


};

#endif