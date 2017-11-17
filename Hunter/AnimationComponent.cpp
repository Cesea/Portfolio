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

		//SKinned Mesh �� Animation �� �����Ѵ�.
		_pSkinnedMesh->_pAnimationController->CloneAnimationController(
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationOutputs(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationSets(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumTracks(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumEvents(), &_pAnimationController);

		//Animation ������ ��´�.
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
			//ũ�ν� ���̵� Ÿ�� ���
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

		//ũ�ν� ���̵尡 �����Ѵٸ�..
		if (_crossFadeTime > 0.0f)
		{
			//���� Animation �� 1 ��Track ���� ����
			_pAnimationController->SetTrackAnimationSet(1, _pPlayingAnimationSet);
			_pAnimationController->SetTrackPosition(1, _playingTrackDesc.Position);	//������ �÷��� �Ǵ� ��ġ�� ����
			_pAnimationController->SetTrackEnable(1, true); //1 �� Track Ȱ��ȭ
			_pAnimationController->SetTrackWeight(1, 1.0f); //1 �� Track ����ġ
			_pAnimationController->SetTrackSpeed(1, _playingTrackDesc.Speed);		//�ӵ� 

			_pAnimationController->SetTrackAnimationSet(0, animation);
			_pAnimationController->SetTrackPosition(0, 0.0f);
			_pAnimationController->SetTrackWeight(0, 0.0f);	//����ġ�� 0 ���� 
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
