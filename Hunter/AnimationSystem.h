#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "AnimationComponent.h"
#include "RenderComponent.h"

#define ANIMATION_CONFIG_MAX_COMPONENT_NUM (1024)

namespace animation
{
	class AnimationSystem : public System<Requires<RenderComponent, AnimationComponent>>
	{
	public:
		AnimationSystem()
			:_animationComponentPool(ANIMATION_CONFIG_MAX_COMPONENT_NUM)
		{}

	private:
		void UpdateAnimation(AnimationComponent &component, float deltaTime, const Matrix &world);
		//void UpdateMesh(AnimationComponent &compoent);

		//void SkinnedAnimation::UpdateAnimation(float deltaTime, const Matrix &world)
		//{

		//}

		ResourceHandlePool<AnimationComponentHandle> _animationComponentPool;
		AnimationComponent _animationComponents[ANIMATION_CONFIG_MAX_COMPONENT_NUM];
	};
}

#endif
