#include "stdafx.h"

bool ActionComponent::CreateFrom(video::AnimationInstanceHandle skinnedAnimation)
{
	Assert(skinnedAnimation.IsValid());
	video::AnimationInstance *pSkinnedAnimation = VIDEO->GetAnimationInstance(skinnedAnimation);

	_pAnimationController = pSkinnedAnimation->_pAnimationController;
	_numAnimation = _pAnimationController->GetNumAnimationSets();

	return true;
}

void ActionComponent::MakeAnimationList()
{
	for (uint32 i = 0; i < _numAnimation; i++)
	{
		LPD3DXANIMATIONSET animSet;
		_pAnimationController->GetAnimationSet(i, &animSet);
		this->_animations.push_back(animSet);
		this->_animationTable.insert(std::make_pair(animSet->GetName(), i));
	}
}

void ActionComponent::SetFirstAction(const Action &action)
{
	auto found = _animationTable.find(action._name);
	if (found != _animationTable.end())
	{
		_playing = true;
		_looping = true;
		_playingAction = action;
		SetAnimation(found->second);
	}
}

void ActionComponent::Destroy()
{
	SAFE_DELETE(_pCallbackHandler);
	COM_RELEASE(_pAnimationController);
}

void ActionComponent::UpdateAnimation(float deltaTime)
{
	_pAnimationController->GetTrackDesc(0, &_playingTrackDesc);
	_animationPlayFactor = _playingTrackDesc.Position / _pPlayingAnimationSet->GetPeriod();

	if (_actionQueue.HasAction())
	{
		const Action &front = _actionQueue.Front();
		if (Play(front))
		{
			_actionQueue.PopAction();
		}
	}
	else 
	{
		if (_animationPlayFactor >= 0.95)
		{
			if (false == _looping)
			{
				//���ư� Animation �� �ִٸ�..
				if (nullptr != _pPrevPlayingAnimationSet)
				{
					_crossFadeTime = _outCrossFadeTime;
					_leftCrossFadeTime = _outCrossFadeTime;
					_looping = true;
					_playingAction = _prevAction;
					SetAnimation(_pPrevPlayingAnimationSet);
					_pPrevPlayingAnimationSet = nullptr;
				}
				else
				{
					this->Stop();
				}
			}
			_animationPlayFactor = _animationPlayFactor - (int32)_animationPlayFactor;
		}
	}

	if (_playing)
	{
		_animDelta = deltaTime;
	}
	else
	{
		_animDelta = 0;
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
			Console::Log("ttt\n");
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

bool ActionComponent::Play(const Action & action)
{
	if (strcmp(action._name, _playingAction._name) == 0)
	{
		return true;
	}

	//���� �÷��� ���� ������ ���ŷ���� �ƴ��� �Ǵ��Ѵ�....
	if (_playingAction._blocking)
	{
		if (_animationPlayFactor > 0.95)
		{
			if (action._playOnce)
			{
				auto found = _animationTable.find(action._name);
				if (found != _animationTable.end())
				{
					_playing = true;
					_looping = false;
					_crossFadeTime = action._crossFadeTime;
					_leftCrossFadeTime = action._crossFadeTime;

					_outCrossFadeTime = action._outCrossFadeTime;

					if ( this->_prevAction._playOnce)
					{
						_pPrevPlayingAnimationSet = _pPlayingAnimationSet;

						this->_prevAction = _playingAction;
						this->_playingAction = action;
					}

					this->SetAnimation(found->second);
					return true;
				}
			}
			else
			{
				auto found = _animationTable.find(action._name);
				if (found != _animationTable.end())
				{
					_playing = true;
					_looping = true;
					_crossFadeTime = action._crossFadeTime;
					_leftCrossFadeTime = action._crossFadeTime;

					this->_playingAction = action;
					this->SetAnimation(found->second);
					return true;
				}
			}
		}
		_animationPlayFactor = _animationPlayFactor - (int32)_animationPlayFactor;
	}
	else
	{
		if (action._playOnce)
		{
		
			auto found = _animationTable.find(action._name);
			if (found != _animationTable.end())
			{

				_playing = true;
				_looping = false;
				_pPrevPlayingAnimationSet = _pPlayingAnimationSet;

				//this->_prevAction = _playingAction;
				//this->_playingAction = action;

				_crossFadeTime = action._crossFadeTime;
				_leftCrossFadeTime = action._crossFadeTime;

				_outCrossFadeTime = action._outCrossFadeTime;

				if (this->_prevAction._playOnce)
				{
					_pPrevPlayingAnimationSet = _pPlayingAnimationSet;

					this->_prevAction = _playingAction;
					this->_playingAction = action;
				}

				this->SetAnimation(found->second);
				return true;
			}
		}
		else
		{
			_playing = true;
			_looping = true;

			auto found = _animationTable.find(action._name);
			if (found != _animationTable.end())
			{
				_crossFadeTime = action._crossFadeTime;
				_leftCrossFadeTime = action._crossFadeTime;

				this->_playingAction = action;
				this->SetAnimation(found->second);
				return true;
			}
		}
	}
	return false;
}

//
//void ActionComponent::Play(int32 animIndex, float crossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	if (animIndex < _numAnimation)
//	{
//		_crossFadeTime = crossFadeTime;
//		_leftCrossFadeTime = crossFadeTime;
//
//		this->SetAnimation(_animations[animIndex]);
//	}
//}
//void ActionComponent::PlayOneShot(int32 animIndex, float inCrossFadeTime, float outCrossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	if (animIndex < _numAnimation)
//	{
//		_pPrevPlayingAnimationSet = _pPlayingAnimationSet;
//
//		_crossFadeTime = inCrossFadeTime;
//		_leftCrossFadeTime = inCrossFadeTime;
//
//		_outCrossFadeTime = outCrossFadeTime;
//
//		this->SetAnimation(_animations[animIndex]);
//	}
//}
//void ActionComponent::PlayOneShotAfterHold(int32 animIndex, float crossFadeTime)
//{
//	_playing = true;
//	_looping = true;
//
//	if (animIndex < _numAnimation)
//	{
//		_pPrevPlayingAnimationSet = nullptr;
//
//		_crossFadeTime = crossFadeTime;
//		_leftCrossFadeTime = crossFadeTime;
//
//		this->SetAnimation(_animations[animIndex]);
//	}
//}
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
//		this->Play(find->second);
//	}
//}
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
//		this->Play(find->second);
//	}
//}
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
//		this->Play(find->second);
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
	_actions[_tail++] = action;
	if (_tail == (ACTION_MAX_NUM))
	{
		_tail = 0;
		Assert(_head != _tail);
	}
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
	_head += 1;
	if (_head == (ACTION_MAX_NUM))
	{
		_head = 0;
	}
}

Action::Action()
{
	ZeroMemory(_name, sizeof(char) * ACTION_MAX_NAME);
	_blocking = true;
	_stop = false;
	_playSpeed = 1.0f;
	_crossFadeTime = 0.0f;
	_outCrossFadeTime = 0.0f;
	_playOnce = false;
}

Action::Action(const Action & other)
	: _blocking(other._blocking), _playSpeed(other._playSpeed), _stop(false),
	_crossFadeTime(other._crossFadeTime), _outCrossFadeTime(other._outCrossFadeTime), _playOnce(other._playOnce)
{
	strncpy(_name, other._name, sizeof(char) * ACTION_MAX_NAME);
}

Action & Action::operator=(const Action & other)
{
	strncpy(_name, other._name, sizeof(char) * ACTION_MAX_NAME);
	_stop = false;
	_blocking = other._blocking;
	_playSpeed = other._playSpeed;
	_crossFadeTime = other._crossFadeTime;
	_outCrossFadeTime = other._outCrossFadeTime;
	_playOnce = other._playOnce;

	return *this;
}

bool AddCallbackKeysAndCompress(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXKEYFRAMEDANIMATIONSET pAnimationSet, 
	DWORD numCallbackKeys, D3DXKEY_CALLBACK * pKeys, DWORD compressionFlags, float compression)
{
	HRESULT hr;
	LPD3DXCOMPRESSEDANIMATIONSET pASNew = NULL;
	LPD3DXBUFFER pBufCompressed = NULL;

	hr = pAnimationSet->Compress(compressionFlags, compression, NULL, &pBufCompressed);
	if (FAILED(hr))
	{
		return false;
	}

	hr = D3DXCreateCompressedAnimationSet(pAnimationSet->GetName(),
		pAnimationSet->GetSourceTicksPerSecond(),
		pAnimationSet->GetPlaybackType(),
		pBufCompressed,
		numCallbackKeys,
		pKeys,
		&pASNew);
	pBufCompressed->Release();

	if (FAILED(hr))
	{
		return false;
	}

	pAnimationController->UnregisterAnimationSet(pAnimationSet);
	pAnimationSet->Release();

	hr = pAnimationController->RegisterAnimationSet(pASNew);
	if (FAILED(hr))
	{
		return false;
	}

	pASNew->Release();
	pASNew = NULL;

	return hr;
}
