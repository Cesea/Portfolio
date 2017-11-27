#include "stdafx.h"
#include "PlayerStates.h"

#include "Player.h"

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
//PlayerCombatState///////////////////////////////////////////////////////////
void PlayerCombatState::OnEnter()
{
	EventChannel channel;

	//_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
}

void PlayerCombatState::Update(float deltaTime, const GameCommand &command)
{
	//switch (command._type)
	//{
	//case GAMECOMMAND_NONE:
	//{
	//}break;
	//case GAMECOMMAND_MOVE:
	//{
	//}break;
	//case GAMECOMMAND_ACTION:
	//{
	//	switch (command._behavior._type)
	//	{
	//		case BEHAVIOR_ATTACK:
	//		{
	//			Attack();
	//			_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
	//		}break;
	//		case BEHAVIOR_SPECIAL_ATTACK:
	//		{
	//		}break;
	//		case BEHAVIOR_SKILL:
	//		{
	//		}break;
	//		case BEHAVIOR_BLOCK:
	//		{
	//		}break;
	//		case BEHAVIOR_INTERACT:
	//		{
	//		}break;
	//	}
	//}break;
	//}
}

void PlayerCombatState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void PlayerCombatState::Attack()
{
}

//Player Move State///////////////////////////////////////////////////////////
void PlayerMoveState::OnEnter()
{
}

void PlayerMoveState::Update(float deltaTime, const GameCommand &command)
{
	//if (_toStanceTimer.Tick(deltaTime))
	//{
	//	_pParent->ChangeState(META_TYPE(PlayerStanceState)->Name());
	//}

	//if (command._type == GAMECOMMAND_MOVE)
	//{
	//	if (command._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
	//	{
	//	}
	//	else if (command._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
	//	{

	//	}

	//	if (command._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
	//	{
	//	}
	//	else if (command._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
	//	{
	//	}
	//}
}

void PlayerMoveState::OnExit()
{
	EventChannel channel;
}

//Player Stance State///////////////////////////////////////////////////////////
void PlayerStanceState::OnEnter()
{
	_pParent->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
}

void PlayerStanceState::Update(float deltaTime, const GameCommand &command)
{
	//switch (command._type)
	//{
	//	case GAMECOMMAND_NONE:
	//	{
	//	}break;
	//	case GAMECOMMAND_MOVE:
	//	{
	//		_pParent->ChangeState(META_TYPE(PlayerMoveState)->Name());
	//	}break;
	//	case GAMECOMMAND_ACTION:
	//	{
	//		switch (command._behavior._type)
	//		{
	//		case BEHAVIOR_NONE:
	//		{
	//		}break;
	//		case BEHAVIOR_ATTACK:
	//		{
	//			_pParent->ChangeState(META_TYPE(PlayerCombatState)->Name());
	//		}break;
	//		case BEHAVIOR_INTERACT:
	//		{
	//		}break;
	//		}
	//	}break;
	//	case GAMECOMMAND_JUMP:
	//	{
	//	}break;
	//	case GAMECOMMAND_INTERACT:
	//	{
	//	}break;
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
