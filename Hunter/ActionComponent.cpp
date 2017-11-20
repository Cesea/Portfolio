#include "stdafx.h"

bool ActionComponent::CreateFrom(video::SkinnedAnimationHandle skinnedAnimation)
{
	Assert(skinnedAnimation.IsValid());
	video::SkinnedAnimation *pSkinnedAnimation = VIDEO->GetSkinnedAnimation(skinnedAnimation);

	pSkinnedAnimation->_pAnimationController->CloneAnimationController(
		pSkinnedAnimation->_pAnimationController->GetMaxNumAnimationOutputs(),
		pSkinnedAnimation->_pAnimationController->GetMaxNumAnimationSets(),
		pSkinnedAnimation->_pAnimationController->GetMaxNumTracks(),
		pSkinnedAnimation->_pAnimationController->GetMaxNumEvents(), &_pAnimationController);

	_numAnimation = _pAnimationController->GetNumAnimationSets();
	for (uint32 i = 0; i < _numAnimation; i++)
	{
		LPD3DXANIMATIONSET animSet;
		_pAnimationController->GetAnimationSet(i, &animSet);
		this->_animations.push_back(animSet);
		this->_animationTable.insert(std::make_pair(animSet->GetName(), animSet));
	}

	this->Play(0);

	return true;
}

void ActionComponent::Destroy()
{
	SAFE_DELETE(_pCallbackHandler);
	COM_RELEASE(_pAnimationController);
}

void ActionComponent::UpdateAnimation(float deltaTime)
{
	//���� ť�� �����ؾ��� �������� �ִ�.....
	if (_actionQueue.HasAction())
	{
		const Action &head = _actionQueue.Front();

		if (head._animationIndex != _playingAnimationIndex)
		{
			this->Play(head._animationIndex, head._crossFadeTime);
			_playingAnimationIndex = head._animationIndex;
			_actionQueue.PopAction();
		}
	}

	_pAnimationController->GetTrackDesc(0, &_playingTrackDesc);
	//���� �󸶳� �Դ���..
	_animationPlayFactor = _playingTrackDesc.Position / _pPlayingAnimationSet->GetPeriod();

	//�������� �����ߴٸ�...
	if (_animationPlayFactor >= 1.0)
	{
		if (false == _looping)
		{
			//���ư� Animation �� �ִٸ�..
			if (nullptr != _pPrevPlayingAnimationSet)
			{
				_crossFadeTime = _outCrossFadeTime;
				_leftCrossFadeTime = _outCrossFadeTime;
				_looping = true;
				SetAnimation(_pPrevPlayingAnimationSet);
				_pPrevPlayingAnimationSet = nullptr;
			}
			else
			{
				this->Stop();
			}
		}
	}

	_animationPlayFactor = _animationPlayFactor - (int32)_animationPlayFactor;

	if (_playing)
	{
		_animDelta = deltaTime;
	}

	//ũ�ν� ���̵尡 �������̶��..
	if (_leftCrossFadeTime > 0.0f)
	{
		//���� ũ�ν����̵� �ð� ����
		_leftCrossFadeTime -= deltaTime;

		//ũ�ν����̵� ��������.
		if (_leftCrossFadeTime <= 0.0f)
		{
			_pAnimationController->SetTrackWeight(0, 1);
			_pAnimationController->SetTrackEnable(1, false);
		}
		else
		{
			float w1 = (_leftCrossFadeTime / _crossFadeTime);		//1�� Track ����ġ
			float w0 = 1.0f - w1;										//0�� Track ����ġ

			_pAnimationController->SetTrackWeight(0, w0);
			_pAnimationController->SetTrackWeight(1, w1);
		}
	}
}


void ActionComponent::Play(int32 animIndex, float crossFadeTime)
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

void ActionComponent::PlayOneShot(int32 animIndex, float inCrossFadeTime, float outCrossFadeTime)
{
	_playing = true;
	_looping = true;

	if (animIndex < _numAnimation)
	{
		_pPrevPlayingAnimationSet = _pPlayingAnimationSet;

		_crossFadeTime = inCrossFadeTime;
		_leftCrossFadeTime = inCrossFadeTime;

		_outCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(_animations[animIndex]);
	}
}

void ActionComponent::PlayOneShotAfterHold(int32 animIndex, float crossFadeTime)
{
	_playing = true;
	_looping = true;

	if (animIndex < _numAnimation)
	{
		_pPrevPlayingAnimationSet = nullptr;

		_crossFadeTime = crossFadeTime;
		_leftCrossFadeTime = crossFadeTime;

		this->SetAnimation(_animations[animIndex]);
	}
}

//void ActionComponent::Play(const std::string & animName, float crossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	AnimationTable::iterator find = _animationTable.find(animName);
//	if (find != _animationTable.end())
//	{
//		//ũ�ν� ���̵� Ÿ�� ���
//		_crossFadeTime = crossFadeTime;
//		_leftCrossFadeTime = crossFadeTime;
//
//		this->SetAnimation(find->second);
//	}
//}
//
//void ActionComponent::PlayOneShot(const std::string & animName, float inCrossFadeTime, float outCrossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	AnimationTable::iterator find = _animationTable.find(animName);
//	if (find != _animationTable.end())
//	{
//		_pPrevPlayingAnimationSet = _pPlayingAnimationSet;
//
//		_crossFadeTime = inCrossFadeTime;
//		_leftCrossFadeTime = inCrossFadeTime;
//
//		_outCrossFadeTime = outCrossFadeTime;
//
//		this->SetAnimation(find->second);
//	}
//}
//
//void ActionComponent::PlayOneShotAfterHold(const std::string & animName, float crossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	AnimationTable::iterator find = _animationTable.find(animName);
//	if (find != _animationTable.end())
//	{
//		_pPrevPlayingAnimationSet = nullptr;
//		_crossFadeTime = crossFadeTime;
//		_leftCrossFadeTime = crossFadeTime;
//		this->SetAnimation(find->second);
//	}
//}


void ActionComponent::SetPlaySpeed(float speed)
{
	_pAnimationController->SetTrackSpeed(0, speed);
}

void ActionComponent::SetAnimation(ID3DXAnimationSet *animationSet)
{
	if ((nullptr != _pPlayingAnimationSet) && (_pPlayingAnimationSet == animationSet))
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

		_pAnimationController->SetTrackAnimationSet(0, animationSet);
		_pAnimationController->SetTrackPosition(0, 0.0f);
		_pAnimationController->SetTrackWeight(0, 0.0f);	//����ġ�� 0 ���� 
		_pPlayingAnimationSet = animationSet;
	}
	else
	{
		_pAnimationController->SetTrackPosition(0, 0.0);
		_pAnimationController->SetTrackAnimationSet(0, animationSet);
		_pPlayingAnimationSet = animationSet;
	}
}

void ActionQueue::PushAction(const Action & action)
{
	if (_tail == (ACTION_MAX_NUM - 1))
	{
		_tail = 0;
		Assert(_head != _tail);
	}

	_actions[_tail++] = action;

}

const Action & ActionQueue::Front()
{
	return _actions[_head];
}

void ActionQueue::PopAction()
{
	if (_head == _tail)
	{
		Console::Log("Error there is no Action in ActionQueue");
		//Assert(false);
	}
	if (_head == (ACTION_MAX_NUM - 1))
	{
		_head = 0;
	}
	_head += 1;
}

Action::Action()
{
	_animationIndex = -1;
	_blocking = true;
	_playSpeed = 1.0f;
	_crossFadeTime = 0.0f;
	_outCrossFadeTime = 0.0f;
	ZeroMemory(_extraInfo, sizeof(char) * 16);
}

Action::Action(const Action & other)
	:_animationIndex(other._animationIndex), _blocking(other._blocking), _playSpeed(other._playSpeed),
	_crossFadeTime(other._crossFadeTime), _outCrossFadeTime(other._outCrossFadeTime)
{
	memcpy(_extraInfo, other._extraInfo, sizeof(char) * 16);
}

Action & Action::operator=(const Action & other)
{
	_animationIndex = other._animationIndex;
	_blocking = other._blocking;
	_playSpeed = other._playSpeed;
	_crossFadeTime = other._crossFadeTime;
	_outCrossFadeTime = other._outCrossFadeTime;
	memcpy(_extraInfo, other._extraInfo, sizeof(char) * 16);

	return *this;
}
