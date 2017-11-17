#include "stdafx.h"
#include "AnimationComponent.h"


namespace animation
{

	AnimationComponent::AnimationComponent()
	{
	}

	AnimationComponent::~AnimationComponent()
	{
	}

	bool AnimationComponent::Create(video::SkinnedXMeshHandle handle)
	{
		if (!handle.IsValid())
		{
			return false;
		}

		_pSkinnedMesh = VIDEO->GetSkinnedXMesh(handle);
		Assert(_pSkinnedMesh);

		_numPalette = _pSkinnedMesh->_numWorkingPalette;
		Assert(_numPalette);
		_workingPalettes = new Matrix[_numPalette];

		//SKinned Mesh 에 Animation 를 복사한다.
		_pSkinnedMesh->_pAnimationController->CloneAnimationController(
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationOutputs(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationSets(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumTracks(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumEvents(), &_pAnimationController);

		//Animation 갯수를 얻는다.
		_numAnimation = _pAnimationController->GetNumAnimationSets();
		for (UINT i = 0; i < _numAnimation; i++)
		{
			LPD3DXANIMATIONSET animSet;
			_pAnimationController->GetAnimationSet(i, &animSet);
			this->_animations.push_back(animSet);
			this->_animationTable.insert(std::make_pair(animSet->GetName(), animSet));
		}

		MatrixIdentity(&_world);

		this->Play(0);
	}

	void AnimationComponent::Destroy()
	{
		COM_RELEASE(_pAnimationController);
	}

	void AnimationComponent::Play(const std::string & animName, float crossFadeTime)
	{
		_playing = true;
		_looping = true;

		AnimationTable::iterator find = _animationTable.find(animName);
		if (find != _animationTable.end())
		{
			//크로스 페이드 타임 기억
			_crossFadeTime = crossFadeTime;
			_leftCrossFadeTime = crossFadeTime;

			this->SetAnimation(find->second);
		}
	}

	void AnimationComponent::Play(int32 animIndex, float crossFadeTime)
	{
		_playing = true;
		_looping = true;

		if (animIndex < _numAnimation)
		{
			_crossFadeTime = crossFadeTime;
			_leftCrossFadeTime = crossFadeTime;

			this->SetAnimation(_animations[animIndex]);
		}
	}

	void AnimationComponent::PlayOneShot(const std::string & animName, float inCrossFadeTime, float outCrossFadeTime)
	{
		_playing = true;
		_looping = true;

		AnimationTable::iterator find = _animationTable.find(animName);
		if (find != _animationTable.end())
		{
			_pPrevPlayAnimationSet = _pPlayingAnimationSet;

			_crossFadeTime = inCrossFadeTime;
			_leftCrossFadeTime = inCrossFadeTime;

			_outCrossFadeTime = outCrossFadeTime;

			this->SetAnimation(find->second);
		}
	}

	void AnimationComponent::PlayOneShotAfterHold(const std::string & animName, float crossFadeTime)
	{
		_playing = true;
		_looping = true;

		AnimationTable::iterator find = _animationTable.find(animName);
		if (find != _animationTable.end())
		{
			_pPrevPlayAnimationSet = nullptr;
			_crossFadeTime = crossFadeTime;
			_leftCrossFadeTime = crossFadeTime;
			this->SetAnimation(find->second);
		}
	}

	void AnimationComponent::SetPlaySpeed(float speed)
	{
		_pAnimationController->SetTrackSpeed(0, speed);
	}

	void AnimationComponent::SetAnimation(LPD3DXANIMATIONSET animation)
	{
		if ((nullptr != _pPlayingAnimationSet) && (_pPlayingAnimationSet == animation))
		{
			return;
		}

		//크로스 페이드가 존재한다면..
		if (_crossFadeTime > 0.0f)
		{
			//현제 Animation 을 1 번Track 으로 셋팅
			_pAnimationController->SetTrackAnimationSet(1, _pPlayingAnimationSet);
			_pAnimationController->SetTrackPosition(1, _playingTrackDesc.Position);	//이전에 플레이 되던 위치로 셋팅
			_pAnimationController->SetTrackEnable(1, true); //1 번 Track 활성화
			_pAnimationController->SetTrackWeight(1, 1.0f); //1 번 Track 가중치
			_pAnimationController->SetTrackSpeed(1, _playingTrackDesc.Speed);		//속도 

			_pAnimationController->SetTrackAnimationSet(0, animation);
			_pAnimationController->SetTrackPosition(0, 0.0f);
			_pAnimationController->SetTrackWeight(0, 0.0f);	//가중치는 0 으로 
			_pPlayingAnimationSet = animation;
		}
		else
		{
			_pAnimationController->SetTrackPosition(0, 0.0);
			_pAnimationController->SetTrackAnimationSet(0, animation);
			_pPlayingAnimationSet = animation;
		}
	}

	void AnimationComponent::UpdateMesh()
	{
		_pAnimationController->AdvanceTime(_animDelta, nullptr);
		_animDelta = 0.0f;
		_pSkinnedMesh->Update(&_world);
	}

}
