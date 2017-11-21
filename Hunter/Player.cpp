#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::CreateFromWorld(World & world)
{
	EventChannel channel;
	channel.Add<InputManager::KeyPressedEvent, Player>(*this);
	_entity = world.CreateEntity();

	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, /*TERRAIN->GetHeight(x * 10, z * 10)*/0, 0);
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Knight"), "Anim" + std::to_string(0));

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Player, Update, *this));

	ActionComponent &actionComp = _entity.AddComponent<ActionComponent>();
	actionComp.CreateFrom(renderComp._skinned);
	actionComp._pCallbackHandler = new PlayerCallbackHandler;

	SetupCallbackAndCompression();
	actionComp.MakeAnimationList();

	Action firstAction;
	firstAction._blocking = false;
	strncpy(firstAction._name, 
		PlayerAnimationString[PlayerAnimationEnum::eStandingFree], strlen(PlayerAnimationString[PlayerAnimationEnum::eStandingFree]));
	actionComp.SetFirstAction(firstAction);


	_entity.Activate();

	_keyConfig._up = 'I';
	_keyConfig._down = 'K';
	_keyConfig._left = 'J';
	_keyConfig._right = 'L';
	_keyConfig._jump = VK_SPACE;
	_keyConfig._special = 'Q';

	_lastCommand._interpreted = true;
}

void Player::Update(float deltaTime)
{
	if (_lastCommand._interpreted == false)
	{
		ActionComponent &refAction = _entity.GetComponent<ActionComponent>();
		refAction._actionQueue.PushAction(std::move(InterpretCommand(_lastCommand)));


		Action test;
		test._blocking = true;
		test._crossFadeTime = 0.2f;
		test._outCrossFadeTime = 0.2f;
		test._playOnce = true;
		strncpy(test._name, PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight]));

		refAction._actionQueue.PushAction(test);

		test._blocking = true;
		test._crossFadeTime = 0.2f;
		test._outCrossFadeTime = 0.2f;
		test._playOnce = true;
		strncpy(test._name, PlayerAnimationString[PlayerAnimationEnum::eWarDodgeToLeft], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarDodgeToLeft]));

		refAction._actionQueue.PushAction(test);

		_lastCommand._interpreted = true;
	}
}

Action Player::InterpretCommand(const Command &command)
{
	Action result;

	if (command._type == Command::Type::eAction)
	{
		result._blocking = true;
		result._crossFadeTime = 0.2f;
		result._outCrossFadeTime = 0.2f;
		result._playOnce = true;
		strncpy(result._name, PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft]));
	}

	return result;
}

void Player::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;
	ID3DXKeyframedAnimationSet *anim1;
	ID3DXKeyframedAnimationSet *anim2;
	ID3DXKeyframedAnimationSet *anim3;
	ID3DXKeyframedAnimationSet *anim4;

	pController->GetAnimationSet(0, (ID3DXAnimationSet **)&anim0);
	pController->GetAnimationSet(1, (ID3DXAnimationSet **)&anim1);
	pController->GetAnimationSet(2, (ID3DXAnimationSet **)&anim2);
	pController->GetAnimationSet(3, (ID3DXAnimationSet **)&anim3);
	pController->GetAnimationSet(4, (ID3DXAnimationSet **)&anim4);

	_callbackData._pPlayerPosition = &refTransform._position;

	D3DXKEY_CALLBACK anim0Keys[2];
	anim0Keys[0].Time = 0;
	anim0Keys[0].pCallbackData = (void *)&_callbackData;
	anim0Keys[1].Time = anim0->GetPeriod() / 2.0f * anim0->GetSourceTicksPerSecond();
	anim0Keys[1].pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	AddCallbackKeysAndCompress(pController, anim1, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	AddCallbackKeysAndCompress(pController, anim2, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	AddCallbackKeysAndCompress(pController, anim3, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	AddCallbackKeysAndCompress(pController, anim4, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);

}

bool Player::AddCallbackKeysAndCompress(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXKEYFRAMEDANIMATIONSET pAnimationSet, 
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

void Player::Handle(const InputManager::KeyPressedEvent & event)
{
	uint32 inputCode = event.code;

	if (_keyConfig._left == inputCode)
	{
		_lastCommand._type = Command::Type::eMove;
		_lastCommand._detail = Command::MoveDirection::eLeft;
		_lastCommand._interpreted = false;
	}
	else if(_keyConfig._right == inputCode)
	{
		_lastCommand._type = Command::Type::eMove;
		_lastCommand._detail = Command::MoveDirection::eRight;
		_lastCommand._interpreted = false;
	}
	else if(_keyConfig._up == inputCode)
	{
		_lastCommand._type = Command::Type::eMove;
		_lastCommand._detail = Command::MoveDirection::eUp;
		_lastCommand._interpreted = false;
	}
	else if (_keyConfig._down == inputCode)
	{
		_lastCommand._type = Command::Type::eMove;
		_lastCommand._detail = Command::MoveDirection::eDown;
		_lastCommand._interpreted = false;
	}
	else if (_keyConfig._jump == inputCode)
	{
	}
	else if(_keyConfig._special == inputCode)
	{
		_lastCommand._type = Command::Type::eAction;
		_lastCommand._detail = Command::ActionType::eAttack;
		_lastCommand._interpreted = false;
	}
}

bool LocalTimer::Tick(float deltaTime)
{
	_currentTime += deltaTime;
	if (_targetTime > 0.0f)
	{
		if (_currentTime > _targetTime)
		{
			return true;
		}
	}
	return false;
}

void LocalTimer::Restart(float targetTime)
{
	_currentTime = 0.0f;
	_targetTime = targetTime;
}
