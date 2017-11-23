#include "stdafx.h"
#include "Player.h"

#include "PlayerStates.h"


Player::Player()
{
}

Player::~Player()
{
}

bool Player::CreateFromWorld(World & world)
{
	EventChannel channel;
	channel.Add<InputManager::KeyPressedEvent, Player>(*this);
	channel.Add<InputManager::KeyDownEvent, Player>(*this);
	channel.Add<InputManager::MousePressedEvent, Player>(*this);
	SetInputConfig();
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
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));

	_entity.Activate();

	_pStateMachine = new PlayerStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(PlayerStanceState)->Name(), new PlayerStanceState());
	_pStateMachine->RegisterState(META_TYPE(PlayerMoveState)->Name(), new PlayerMoveState());
	_pStateMachine->RegisterState(META_TYPE(PlayerAttackState)->Name(), new PlayerAttackState());
	_pStateMachine->RegisterState(META_TYPE(PlayerCombatState)->Name(), new PlayerCombatState());
	_pStateMachine->RegisterState(META_TYPE(PlayerDeadState)->Name(), new PlayerDeadState());
	_pStateMachine->ChangeState(META_TYPE(PlayerStanceState)->Name());

	_currentCommand._interpreted = true;

	return true;
}

void Player::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	_currentCommand.Reset();
}

void Player::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;
	//ID3DXKeyframedAnimationSet *anim1;
	//ID3DXKeyframedAnimationSet *anim2;
	//ID3DXKeyframedAnimationSet *anim3;
	//ID3DXKeyframedAnimationSet *anim4;

	pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], (ID3DXAnimationSet **)&anim0);

	//pController->GetAnimationSet(0, (ID3DXAnimationSet **)&anim0);
	//pController->GetAnimationSet(1, (ID3DXAnimationSet **)&anim1);
	//pController->GetAnimationSet(2, (ID3DXAnimationSet **)&anim2);
	//pController->GetAnimationSet(3, (ID3DXAnimationSet **)&anim3);
	//pController->GetAnimationSet(4, (ID3DXAnimationSet **)&anim4);

	_callbackData._animtionEnum = (PlayerAnimationEnum *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;
	//anim0Keys[0].Time = 0;
	//anim0Keys[0].pCallbackData = (void *)&_callbackData;
	//anim0Keys[1].Time = anim0->GetPeriod() / 2.0f * anim0->GetSourceTicksPerSecond();
	//anim0Keys[1].pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim1, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim2, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim3, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
	//AddCallbackKeysAndCompress(pController, anim4, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Player::Handle(const InputManager::KeyPressedEvent & event)
{
}

void Player::Handle(const InputManager::MousePressedEvent & event)
{
	uint32 inputCode = event.code;

	if (inputCode == _inputConfig._attack)
	{
		_currentCommand._type = GAMECOMMAND_ACTION;
		_currentCommand._behavior._type = BEHAVIOR_ATTACK;
		//_currentCommand._interpreted = false;
		//_channel.Broadcast<Player::AttackEvent>(AttackEvent());
	}
	else if (inputCode == _inputConfig._block)
	{
		_currentCommand._type = GAMECOMMAND_ACTION;
		_currentCommand._behavior._type = BEHAVIOR_BLOCK;
		//_currentCommand._interpreted = false;
	}
}

void Player::Handle(const InputManager::KeyDownEvent & event)
{
	uint32 inputCode = event.code;

	if (_inputConfig._left == inputCode)
	{
		_currentCommand._type = GAMECOMMAND_MOVE;
		_currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
		//_currentCommand._behavior._type = Behavior::Type::eWalk;
		//_currentCommand._interpreted = false;
		//_channel.Broadcast<Player::MoveEvent>(Player::MoveEvent());
	}
	else if(_inputConfig._right == inputCode)
	{
		_currentCommand._type = GAMECOMMAND_MOVE;
		_currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
		//_currentCommand._behavior._type = Behavior::Type::eWalk;
		//_currentCommand._interpreted = false;
		//_channel.Broadcast<Player::MoveEvent>(Player::MoveEvent());
	}
	else if(_inputConfig._up == inputCode)
	{
		_currentCommand._type = GAMECOMMAND_MOVE;
		_currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
		//_currentCommand._behavior._type = Behavior::Type::eWalk;
		//_currentCommand._interpreted = false;
		//_channel.Broadcast<Player::MoveEvent>(Player::MoveEvent());
	}
	else if (_inputConfig._down == inputCode)
	{
		_currentCommand._type = GAMECOMMAND_MOVE;
		_currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
		//_currentCommand._behavior._type = Behavior::Type::eWalk;
		//_currentCommand._interpreted = false;
		//_channel.Broadcast<Player::MoveEvent>(Player::MoveEvent());
	}
	else if (_inputConfig._jump == inputCode)
	{
		_currentCommand._type = GAMECOMMAND_JUMP;
	}

}

void Player::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
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

