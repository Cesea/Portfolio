#include "stdafx.h"
#include "AnimationSystem.h"

namespace animation
{
	void AnimationSystem::UpdateAnimation(AnimationComponent & component, float deltaTime, const Matrix & world)
	{
		component._pAnimationController->GetTrackDesc(0, &component._playingTrackDesc);
		//���� �󸶳� �Դ���..
		component._animationPlayFactor =
			component._playingTrackDesc.Position / component._pPlayingAnimationSet->GetPeriod();

		//�������� �����ߴٸ�...
		if (component._animationPlayFactor >= 1.0)
		{
			if (false == component._looping)
			{
				//���ư� Animation �� �ִٸ�..
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

		//ũ�ν� ���̵尡 �������̶��..
		if (component._leftCrossFadeTime > 0.0f)
		{
			//���� ũ�ν����̵� �ð� ����
			component._leftCrossFadeTime -= deltaTime;

			//ũ�ν����̵� ��������.
			if (component._leftCrossFadeTime <= 0.0f)
			{
				component._pAnimationController->SetTrackWeight(0, 1);
				component._pAnimationController->SetTrackEnable(1, false);
			}
			else
			{
				float w1 = (component._leftCrossFadeTime / component._crossFadeTime);		//1�� Track ����ġ
				float w0 = 1.0f - w1;										//0�� Track ����ġ

				component._pAnimationController->SetTrackWeight(0, w0);
				component._pAnimationController->SetTrackWeight(1, w1);
			}
		}

		component._world = world;
	}
}