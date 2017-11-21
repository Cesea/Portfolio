#include "stdafx.h"
#include "PlayerStates.h"

#include "Player.h"

DEFINE_META(PlayerAttackState)
{
	ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerStanceState)
{
	ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerCombatState)
{
	ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerMoveState)
{
	ADD_MEMBER(_pActor);
}

DEFINE_META(PlayerDeadState)
{
	ADD_MEMBER(_pActor);
}

//Player State /////////////////////////////////////////
bool PlayerState::Init(Player * pPlayer)
{
	_pActor = pPlayer;
	return true;
}

void PlayerState::Release()
{
	_pActor = nullptr;
}
void PlayerAttackState::OnEnter()
{}
void PlayerAttackState::Update(float deltaTime, const GameCommand &command)
{}
void PlayerAttackState::OnExit()
{}

void PlayerCombatState::OnEnter()
{}
void PlayerCombatState::Update(float deltaTime, const GameCommand &command)
{}
void PlayerCombatState::OnExit()
{}

void PlayerMoveState::OnEnter()
{}
void PlayerMoveState::Update(float deltaTime, const GameCommand &command)
{}
void PlayerMoveState::OnExit()
{}

void PlayerDeadState::OnEnter()
{}
void PlayerDeadState::Update(float deltaTime, const GameCommand &command)
{}
void PlayerDeadState::OnExit()
{}



void PlayerStanceState::OnEnter()
{
	_randomTimer.Restart(RandFloat(3.0f, 5.0f));
	_pActor->_pActionComp->SetFirstAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
	//_channel.Broadcast<Player::QueueActionEvent>(Player::QueueActionEvent());
}

void PlayerStanceState::Update(float deltaTime, const GameCommand &command)
{
	if (_randomTimer.Tick(deltaTime))
	{
		int32 randomNumber = RandInt(0, 30);
		if (randomNumber >= 0 && randomNumber < 10)
		{
			_channel.Broadcast<Player::QueueActionEvent>(Player::QueueActionEvent(PLAYER_ANIM(PlayerAnimationEnum::eSalute)));
		}
		else if (randomNumber >= 10 && randomNumber < 20)
		{
			_channel.Broadcast<Player::QueueActionEvent>(Player::QueueActionEvent(PLAYER_ANIM(PlayerAnimationEnum::eBoring)));
		}
		else if (randomNumber >= 20 && randomNumber < 30)
		{
			_channel.Broadcast<Player::QueueActionEvent>(Player::QueueActionEvent(PLAYER_ANIM(PlayerAnimationEnum::eLookingAround)));
		}
		_randomTimer.Restart(RandFloat(3.0f, 5.0f));

	}
}

void PlayerStanceState::OnExit()
{
}

void PlayerStanceState::Handle(const Player::MoveEvent & event)
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
