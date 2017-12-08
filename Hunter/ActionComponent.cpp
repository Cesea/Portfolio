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
		_animations.push_back(animSet);
		_animationTable.insert(std::make_pair(animSet->GetName(), i));
	}
}

void ActionComponent::SetFirstAction(const Action &action)
{
	auto found = _animationTable.find(action._name);
	if (found != _animationTable.end())
	{
		_playing = true;
		_looping = true;
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
	_pAnimationController->GetTrackDesc( 0, &_playingTrackDesc );
	_animationPlayFactor = _playingTrackDesc.Position / _pPlayingAnimationSet->GetPeriod();

	if (!_blocking)
	{
		if (_actionQueue.HasAction())
		{
			PlayActionImmediate(_actionQueue.Front());
			_actionQueue.PopAction();
		}
	}
	else
	{
		//NOTE : 여기서 cancle 처리를 한다....
		if (_actionQueue.HasAction() && 
			_actionQueue.Front()._cancle)
		{
			//PlayActionImmediate(_actionQueue.Front());
			//_actionQueue.PopAction();
		}
		else if (_animationPlayFactor >= 0.95f)
		{
			if (_actionQueue.HasAction())
			{
				PlayActionImmediate(_actionQueue.Front());
				_actionQueue.PopAction();
			}
			else if (false == _looping)
			{
				if (nullptr != _pPrevPlayingAnimationSet)
				{
					_crossFadeTime = _outCrossFadeTime;
					_leftCrossFadeTime = _outCrossFadeTime;
					_blocking = false;
					_looping = true;
					SetAnimation(_pPrevPlayingAnimationSet);
					_pPrevPlayingAnimationSet = nullptr;
				}
				else
				{
					this->Stop();
					_blocking = false;
				}
			}
		}
	}

	_animationPlayFactor = _animationPlayFactor - (int32)_animationPlayFactor;	//정수부분 안생기게....

	if ( _playing )
	{
		_animDelta = deltaTime;
	}
	else
	{
		_animDelta = 0.0f;
	}

	if ( _leftCrossFadeTime > 0.0f )
	{
		_leftCrossFadeTime -= deltaTime;
		if ( _leftCrossFadeTime <= 0.0f )
		{
			_pAnimationController->SetTrackWeight( 0, 1 );
			_pAnimationController->SetTrackEnable( 1, false );
		}
		else
		{
			float w1 = ( _leftCrossFadeTime / _crossFadeTime );	
			float w0 = 1.0f - w1;
			_pAnimationController->SetTrackWeight( 0, w0 );
			_pAnimationController->SetTrackWeight( 1, w1 );
		}
	}
}

bool ActionComponent::PlayActionImmediate(const Action & action)
{
	if (action._stop && action._playOnce)
	{
		PlayOneShotAfterHold(action._name, action._crossFadeTime, action._blocking);
	}
	else
	{
		if (action._playOnce)
		{
			PlayOneShot(action._name, action._crossFadeTime, action._outCrossFadeTime, action._blocking);
		}
		else
		{
			Play(action._name, action._crossFadeTime, action._blocking);
		}
	}
	return true;
}

//Play Functions ////////////////////////////////////////////////////////////
void ActionComponent::Play(const std::string & animName, float crossFadeTime, bool32 blocking)
{
	_playing = true;
	_prevLooping = _looping;
	_looping = true;

	_prevBlocking = _blocking;
	_blocking = blocking;

	AnimationSetTable::iterator find = _animationTable.find( animName );
	if ( find != this->_animationTable.end() ) 
	{
		_crossFadeTime = crossFadeTime;
		_leftCrossFadeTime = crossFadeTime;

		this->SetAnimation( find->second );
	}
}

void ActionComponent::Play(int32 animIndex, float crossFadeTime, bool32 blocking)
{
	_playing = true;
	_prevLooping = _looping;
	_looping = true;

	_prevBlocking = _blocking;
	_blocking = blocking;

	if ( animIndex < _numAnimation ) 
	{
		_crossFadeTime = crossFadeTime;
		_leftCrossFadeTime = crossFadeTime;

		this->SetAnimation( _animations[animIndex] );
	}
}

void ActionComponent::Play(LPD3DXANIMATIONSET animSet, float crossFadeTime, bool32 blocking)
{
	_playing = true;
	_prevLooping = _looping;
	_looping = true;

	_prevBlocking = _blocking;
	_blocking = blocking;

	_crossFadeTime = crossFadeTime;
	_leftCrossFadeTime = crossFadeTime;

	this->SetAnimation(animSet);
}

void ActionComponent::PlayOneShot(const std::string &animName, 
	float inCrossFadeTime, float outCrossFadeTime, bool32 blocking)
{
	_playing = true;
	_prevLooping = _looping;
	_looping = false;

	_prevBlocking = _blocking;
	_blocking = blocking;

	AnimationSetTable::iterator find = _animationTable.find( animName );
	if ( find != _animationTable.end() ) 
	{
		if (_prevLooping)
		{
			_pPrevPlayingAnimationSet = _pPlayingAnimationSet;
		}

		_crossFadeTime = inCrossFadeTime;
		_leftCrossFadeTime = inCrossFadeTime;

		_outCrossFadeTime = outCrossFadeTime;

		this->SetAnimation( find->second );
	}
}

void ActionComponent::PlayOneShotAfterHold(const std::string & animName, float crossFadeTime, bool32 blocking)
{
	_playing = true;

	_prevLooping = _looping;
	_looping = false;

	_prevBlocking = _blocking;
	_blocking = blocking;

	AnimationSetTable::iterator find = _animationTable.find( animName );
	if ( find != _animationTable.end() ) 
	{
		_pPrevPlayingAnimationSet = nullptr;

		_crossFadeTime = crossFadeTime;
		_leftCrossFadeTime = crossFadeTime;

		this->SetAnimation( find->second );
	}
}

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

	//크로스 페이드가 존재한다면..
	if (_crossFadeTime > 0.0f)
	{
		//현제 Animation 을 1 번Track 으로 셋팅
		_pAnimationController->SetTrackAnimationSet(1, _pPlayingAnimationSet);
		_pAnimationController->SetTrackPosition(1, _playingTrackDesc.Position);	//이전에 플레이 되던 위치로 셋팅
		_pAnimationController->SetTrackEnable(1, true); //1 번 Track 활성화
		_pAnimationController->SetTrackWeight(1, 1.0f); //1 번 Track 가중치
		_pAnimationController->SetTrackSpeed(1, _playingTrackDesc.Speed);		//속도 

		_pAnimationController->SetTrackAnimationSet(0, animationSet);
		_pAnimationController->SetTrackPosition(0, 0.0f);
		_pAnimationController->SetTrackWeight(0, 0.0f);	//가중치는 0 으로 
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
	_enum = 0;
	_cancle = false;
}

Action::Action(const Action & other)
	: _blocking(other._blocking), _playSpeed(other._playSpeed), _stop(other._stop),
	_crossFadeTime(other._crossFadeTime), _outCrossFadeTime(other._outCrossFadeTime), _playOnce(other._playOnce), 
	_enum(other._enum), _cancle(other._cancle)
{
	strncpy(_name, other._name, sizeof(char) * ACTION_MAX_NAME);
}

Action & Action::operator=(const Action & other)
{
	strncpy(_name, other._name, sizeof(char) * ACTION_MAX_NAME);
	_stop = other._stop;
	_blocking = other._blocking;
	_playSpeed = other._playSpeed;
	_crossFadeTime = other._crossFadeTime;
	_outCrossFadeTime = other._outCrossFadeTime;
	_playOnce = other._playOnce;
	_enum = other._enum;
	_cancle = other._cancle;

	return *this;
}

bool AddCallbackKeysAndCompress(LPD3DXANIMATIONCONTROLLER pAnimationController, 
	LPD3DXKEYFRAMEDANIMATIONSET pAnimationSet, 
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
