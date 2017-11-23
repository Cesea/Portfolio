#include "stdafx.h"
#include "PlayerStates.h"

#include "Player.h"

DEFINE_META(PlayerAttackState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerStanceState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerCombatState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerMoveState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerDeadState)
{
	//ADD_MEMBER(_pActor);
}

//Player State /////////////////////////////////////////
bool PlayerState::Init(StateMachine<Player> * pParent)
{
	_pParent = pParent;
	return true;
}

void PlayerState::Release()
{
	_pParent = nullptr;
}

//PlayerAttackState///////////////////////////////////////////////////////////
void PlayerAttackState::OnEnter()
{
	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
}

void PlayerAttackState::Update(float deltaTime, const GameCommand &command)
{
}

void PlayerAttackState::OnExit()
{
}

//PlayerCombatState///////////////////////////////////////////////////////////
void PlayerCombatState::OnEnter()
{
	EventChannel channel;
	channel.Add<Player::AttackEvent, PlayerCombatState>(*this);
	channel.Add<Player::MoveEvent, PlayerCombatState>(*this);

	_toStanceTimer.Restart(3.0f);
	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
}

void PlayerCombatState::Update(float deltaTime, const GameCommand &command)
{
	switch (command._type)
	{
	case GameCommand::Type::eNone:
	{
	}break;
	case GameCommand::Type::eMove:
	{
	}break;
	case GameCommand::Type::eAction:
	{
		switch (command._behavior._type)
		{
			case Behavior::Type::eAttack:
			{
				_pParent->ChangeState(META_TYPE(PlayerAttackState)->Name());
			}break;
			case Behavior::Type::eSpecialAttack:
			{
			}break;
			case Behavior::Type::eSkill:
			{
			}break;
			case Behavior::Type::eBlock:
			{
			}break;
			case Behavior::Type::eInteract:
			{
			}break;
		}
	}break;
	}
}

void PlayerCombatState::OnExit()
{
	_toStanceTimer.Restart(3.0f);
	_pParent->ClearActioniQueue();
}

//void PlayerCombatState::Handle(const Player::AttackEvent & event)
//{
//	//_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
//	_pParent->ChangeState(META_TYPE(PlayerAttackState)->Name());
//	_toStanceTimer.Restart(2.0f);
//}
//
//void PlayerCombatState::Handle(const Player::MoveEvent & event)
//{
//	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
//	_toStanceTimer.Restart(2.0f);
//}

//Player Move State///////////////////////////////////////////////////////////
void PlayerMoveState::OnEnter()
{
	EventChannel channel;
	channel.Add<Player::MoveEvent, PlayerMoveState>(*this);

	_toStanceTimer.Restart(1.0f);
	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
}

void PlayerMoveState::Update(float deltaTime, const GameCommand &command)
{
	if (_toStanceTimer.Tick(deltaTime))
	{
		_pParent->ChangeState(META_TYPE(PlayerStanceState)->Name());
	}
}

void PlayerMoveState::OnExit()
{
	EventChannel channel;
	channel.Remove<Player::MoveEvent, PlayerMoveState>(*this);
}

//void PlayerMoveState::Handle(const Player::MoveEvent & event)
//{
//	_toStanceTimer.Restart(1.0f);
//}

//Player Stance State///////////////////////////////////////////////////////////
void PlayerStanceState::OnEnter()
{
	EventChannel channel;
	channel.Add<Player::MoveEvent, PlayerStanceState>(*this);
	channel.Add<Player::AttackEvent, PlayerStanceState>(*this);

	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
}

void PlayerStanceState::Update(float deltaTime, const GameCommand &command)
{
	switch (command._type)
	{
		case GameCommand::Type::eNone:
		{
		}break;
		case GameCommand::Type::eMove:
		{
		}break;
		case GameCommand::Type::eAction:
		{
			switch (command._behavior._type)
			{
			case Behavior::Type::eNone:
			{
			}break;
			case Behavior::Type::eAttack:
			{
				_pParent->ChangeState(META_TYPE(PlayerCombatState)->Name());
			}break;
			case Behavior::Type::eInteract:
			{
			}break;
			}
		}break;
		case GameCommand::Type::eJump:
		{
		}break;
		case GameCommand::Type::eInteract:
		{
		}break;
	}
	//if (_randomTimer.Tick(deltaTime))
	//{
	//	int32 randomNumber = RandInt(0, 30);
	//	if (randomNumber >= 0 && randomNumber < 10)
	//	{
	//		_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eSalute));
	//	}
	//	else if (randomNumber >= 10 && randomNumber < 20)
	//	{
	//		_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eBoring));
	//	}
	//	else if (randomNumber >= 20 && randomNumber < 30)
	//	{
	//		_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eLookingAround));
	//	}
	//	_randomTimer.Restart(RandFloat(5.0f, 7.0f));
	//}
}

void PlayerStanceState::OnExit()
{
	_pParent->ClearActioniQueue();
}

//void PlayerStanceState::Handle(const Player::AttackEvent & event)
//{
//	Console::Log("Combat Begin\n");
//	_pParent->ChangeState(META_TYPE(PlayerCombatState)->Name());
//}
//
//void PlayerStanceState::Handle(const Player::MoveEvent & event)
//{
//	Console::Log("I'm moving\n");
//
//	_pParent->ChangeState(META_TYPE(PlayerMoveState)->Name());
//}


//Player Dead State///////////////////////////////////////////////////////////
void PlayerDeadState::OnEnter()
{
}

void PlayerDeadState::Update(float deltaTime, const GameCommand &command)
{
}

void PlayerDeadState::OnExit()
{
}


////Player Normal State //////////////////////////////////
//void PlayerNormalState::OnEnter()
//{
//	//시작 엑션을 밀어준다
//	Action firstAction;
//	firstAction._blocking = false;
//	strncpy(firstAction._name, 
//		PlayerAnimationString[PlayerAnimationEnum::eStandingFree], strlen(PlayerAnimationString[PlayerAnimationEnum::eStandingFree]));
//	_pActor->_pActionComp->SetFirstAction(firstAction);
//
//
//	_idleRandomTimer.Restart(2.0f);
//
//}
//
//PlayerState * PlayerNormalState::Update(float deltaTime, const GameCommand &command)
//{
//	PlayerState *newState = nullptr;
//	//인풋이 없었다.
//	if (true == command._interpreted)
//	{
//		if (_lastCommand._type == GameCommand::Type::eMove)
//		{
//			_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
//		}
//	}
//	else if (false == command._interpreted)
//	{
//		if (command._type == GameCommand::Type::eMove)
//		{
//			if (command._movement._vertical == Movement::Vertical::eUp)
//			{
//				_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
//			}
//			else if (command._movement._vertical == Movement::Vertical::eDown)
//			{
//				_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
//			}
//		}
//		else if (command._type == GameCommand::Type::eJump)
//		{
//			_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eJumpRunning));
//		}
//	}
//
//	//if (_idleRandomTimer.Tick(deltaTime))
//	//{
//	//	_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eSitDown));
//	//}
//
//	_lastCommand = command;
//	return newState;
//}
//
//void PlayerNormalState::OnExit()
//{
//}
//
//void PlayerWarState::OnEnter()
//{
//}
//
//PlayerState * PlayerWarState::Update(float deltaTime, const GameCommand &command)
//{
//	PlayerState *newState = nullptr;
//
//
//	_lastCommand = command;
//	return newState;
//}
//
//void PlayerWarState::OnExit()
//{
//}
//
//void PlayerDeadState::OnEnter()
//{
//}
//
//PlayerState * PlayerDeadState::Update(float deltaTime, const GameCommand &command)
//{
//	PlayerState *newState = nullptr;
//
//
//	_lastCommand = command;
//	return newState;
//}
//
//void PlayerDeadState::OnExit()
//{
//}
