#include "stdafx.h"
#include "AnimationSystem.h"

namespace animation
{
	void AnimationSystem::UpdateAnimation(AnimationComponent & component, float deltaTime, const Matrix & world)
	{
		component._pAnimationController->GetTrackDesc(0, &component._playingTrackDesc);
		//현재 얼마나 왔는지..
		component._animationPlayFactor =
			component._playingTrackDesc.Position / component._pPlayingAnimationSet->GetPeriod();

		//마지막에 도달했다면...
		if (component._animationPlayFactor >= 1.0)
		{
			if (false == component._looping)
			{
				//돌아갈 Animation 이 있다면..
				if (nullptr != component._pPrevPlayAnimationSet)
				{
					component._crossFadeTime = component._outCrossFadeTime;
					component._leftCrossFadeTime = component._outCrossFadeTime;
					component._looping = true;
					component.SetAnimation(component._pPrevPlayAnimationSet);
					component._pPrevPlayAnimationSet = nullptr;
				}
				else
				{
					component.Stop();
				}
			}
		}

		component._animationPlayFactor = component._animationPlayFactor - (int32)component._animationPlayFactor;

		if (component._playing)
		{
			component._animDelta = deltaTime;
		}

		//크로스 페이드가 진행중이라면..
		if (component._leftCrossFadeTime > 0.0f)
		{
			//남은 크로스페이드 시간 뺀다
			component._leftCrossFadeTime -= deltaTime;

			//크로스페이드 가끝났다.
			if (component._leftCrossFadeTime <= 0.0f)
			{
				component._pAnimationController->SetTrackWeight(0, 1);
				component._pAnimationController->SetTrackEnable(1, false);
			}
			else
			{
				float w1 = (component._leftCrossFadeTime / component._crossFadeTime);		//1번 Track 가중치
				float w0 = 1.0f - w1;										//0번 Track 가중치

				component._pAnimationController->SetTrackWeight(0, w0);
				component._pAnimationController->SetTrackWeight(1, w1);
			}
		}

		component._world = world;
	}
}