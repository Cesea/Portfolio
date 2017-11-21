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
	channel.Add<InputManager::MousePressedEvent, Player>(*this);
	_entity = world.CreateEntity();

	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, /*TERRAIN->GetHeight(x * 10, z * 10)*/0, 0);
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Knight"), "Anim" + std::to_string(0));

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Player, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new PlayerCallbackHandler;

	SetupCallbackAndCompression();
	_pActionComp->MakeAnimationList();


	_entity.Activate();

	_stateMachine.Init(this);
	_stateMachine.ChangeState(new PlayerNormalState);

	_lastCommand._interpreted = true;
}

void Player::Update(float deltaTime)
{
	_stateMachine.Update(deltaTime, _lastCommand);

	//if (_lastCommand._interpreted == false)
	//{
	//	ActionComponent &refAction = _entity.GetComponent<ActionComponent>();
	//	//refAction._actionQueue.PushAction(std::move(InterpretCommand(_lastCommand)));
	//	Action test;
	//	test._blocking = true;
	//	test._crossFadeTime = 0.2f;
	//	test._outCrossFadeTime = 0.2f;
	//	test._playOnce = true;
	//	strncpy(test._name, PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarSwingRight]));
	//	refAction._actionQueue.PushAction(test);
	//	test._blocking = true;
	//	test._crossFadeTime = 0.2f;
	//	test._outCrossFadeTime = 0.2f;
	//	test._playOnce = true;
	//	memset(test._name, 0, sizeof(char) * ACTION_MAX_NAME);
	//	strncpy(test._name, PlayerAnimationString[PlayerAnimationEnum::eWarDodgeToLeft], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarDodgeToLeft]));
	//	refAction._actionQueue.PushAction(test);
	//	test._blocking = false;
	//	test._crossFadeTime = 0.2f;
	//	test._outCrossFadeTime = 0.2f;
	//	test._playOnce = false;
	//	memset(test._name, 0, sizeof(char) * ACTION_MAX_NAME);
	//	strncpy(test._name, PlayerAnimationString[PlayerAnimationEnum::eWarCombatMode], strlen(PlayerAnimationString[PlayerAnimationEnum::eWarCombatMode]));
	//	refAction._actionQueue.PushAction(test);
	//	_lastCommand._interpreted = true;
	//}
}

void Player::SetupCallbackAndCompression()
{
	//ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	//TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	//ID3DXAnimationController *pController = refActionComp._pAnimationController;
	//uint32 numAnimationSet = pController->GetNumAnimationSets();
	//ID3DXKeyframedAnimationSet *anim0;
	//ID3DXKeyframedAnimationSet *anim1;
	//ID3DXKeyframedAnimationSet *anim2;
	//ID3DXKeyframedAnimationSet *anim3;
	//ID3DXKeyframedAnimationSet *anim4;

	//pController->GetAnimationSet(0, (ID3DXAnimationSet **)&anim0);
	//pController->GetAnimationSet(1, (ID3DXAnimationSet **)&anim1);
	//pController->GetAnimationSet(2, (ID3DXAnimationSet **)&anim2);
	//pController->GetAnimationSet(3, (ID3DXAnimationSet **)&anim3);
	//pController->GetAnimationSet(4, (ID3DXAnimationSet **)&anim4);

	//_callbackData._pPlayerPosition = &refTransform._position;

	//D3DXKEY_CALLBACK anim0Keys[2];
	//anim0Keys[0].Time = 0;
	//anim0Keys[0].pCallbackData = (void *)&_callbackData;
	//anim0Keys[1].Time = anim0->GetPeriod() / 2.0f * anim0->GetSourceTicksPerSecond();
	//anim0Keys[1].pCallbackData = (void *)&_callbackData;

	//AddCallbackKeysAndCompress(pController, anim0, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim1, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim2, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim3, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim4, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
}



void Player::Handle(const InputManager::KeyPressedEvent & event)
{
	uint32 inputCode = event.code;

	if (_inputConfig._left == inputCode)
	{
		_lastCommand._type = GameCommand::Type::eMove;
		_lastCommand._movement._horizontal = Movement::Horizontal::eLeft;
		_lastCommand._behavior._type = Behavior::Type::eWalk;
		_lastCommand._interpreted = false;
	}
	else if(_inputConfig._right == inputCode)
	{
		_lastCommand._type = GameCommand::Type::eMove;
		_lastCommand._movement._horizontal = Movement::Horizontal::eRight;
		_lastCommand._behavior._type = Behavior::Type::eWalk;
		_lastCommand._interpreted = false;
	}
	else if(_inputConfig._up == inputCode)
	{
		_lastCommand._type = GameCommand::Type::eMove;
		_lastCommand._movement._vertical = Movement::Vertical::eUp;
		_lastCommand._behavior._type = Behavior::Type::eWalk;
		_lastCommand._interpreted = false;
	}
	else if (_inputConfig._down == inputCode)
	{
		_lastCommand._type = GameCommand::Type::eMove;
		_lastCommand._movement._vertical = Movement::Vertical::eDown;
		_lastCommand._behavior._type = Behavior::Type::eWalk;
		_lastCommand._interpreted = false;
	}
	//else if (_keyConfig._jump == inputCode)
	//{
	//	_lastCommand._type = GameCommand::Type::eJump;
	//	_lastCommand._behavior._type = Behavior::Type::eJump;
	//	//_lastCommand._movement._vertical = Movement::Vertical::eDown;
	//	_lastCommand._interpreted = false;
	//}
	else if(_inputConfig._special == inputCode)
	{
		_lastCommand._type = GameCommand::Type::eAction;
		_lastCommand._interpreted = false;
	}
}

void Player::Handle(const InputManager::MousePressedEvent & event)
{
	uint32 inputCode = event.code;

	if (inputCode == _inputConfig._attack)
	{
		_lastCommand._type = GameCommand::Type::eAction;
		_lastCommand._behavior._type = Behavior::Type::eAttack;
		_lastCommand._interpreted = false;
	}
	else if (inputCode == _inputConfig._block)
	{
		_lastCommand._type = GameCommand::Type::eAction;
		_lastCommand._behavior._type = Behavior::Type::eBlock;
		_lastCommand._interpreted = false;
	}
}

void Player::SetInputConfig()
{

	_inputConfig._up = 'I';
	_inputConfig._down = 'K';
	_inputConfig._left = 'J';
	_inputConfig._right = 'L';
	_inputConfig._jump = VK_SPACE;
	_inputConfig._sneak = VK_CONTROL;

	_inputConfig._attack = MOUSE_BUTTON_LEFT;
	_inputConfig._block = MOUSE_BUTTON_RIGHT;

	_inputConfig._special = 'U';
}

