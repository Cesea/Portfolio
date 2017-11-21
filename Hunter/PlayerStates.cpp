#include "stdafx.h"
#include "PlayerStates.h"

#include "Player.h"

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

//Player Normal State //////////////////////////////////
void PlayerNormalState::OnEnter()
{
	//시작 엑션을 밀어준다
	Action firstAction;
	firstAction._blocking = false;
	strncpy(firstAction._name, 
		PlayerAnimationString[PlayerAnimationEnum::eStandingFree], strlen(PlayerAnimationString[PlayerAnimationEnum::eStandingFree]));
	_pActor->_pActionComp->SetFirstAction(firstAction);


	_idleRandomTimer.Restart(2.0f);

}

PlayerState * PlayerNormalState::Update(float deltaTime, const GameCommand &command)
{
	PlayerState *newState = nullptr;
	//인풋이 없었다.
	if (true == command._interpreted)
	{
		if (_lastCommand._type == GameCommand::Type::eMove)
		{
			_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
		}
	}
	else if (false == command._interpreted)
	{
		if (command._type == GameCommand::Type::eMove)
		{
			if (command._movement._vertical == Movement::Vertical::eUp)
			{
				_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
			}
			else if (command._movement._vertical == Movement::Vertical::eDown)
			{
				_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
			}
		}
		else if (command._type == GameCommand::Type::eJump)
		{
			_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eJumpRunning));
		}
	}

	//if (_idleRandomTimer.Tick(deltaTime))
	//{
	//	_pActor->_pActionComp->_actionQueue.PushAction(PLAYER_ANIM(PlayerAnimationEnum::eSitDown));
	//}

	_lastCommand = command;
	return newState;
}

void PlayerNormalState::OnExit()
{
}

void PlayerWarState::OnEnter()
{
}

PlayerState * PlayerWarState::Update(float deltaTime, const GameCommand &command)
{
	PlayerState *newState = nullptr;


	_lastCommand = command;
	return newState;
}

void PlayerWarState::OnExit()
{
}

void PlayerDeadState::OnEnter()
{
}

PlayerState * PlayerDeadState::Update(float deltaTime, const GameCommand &command)
{
	PlayerState *newState = nullptr;


	_lastCommand = command;
	return newState;
}

void PlayerDeadState::OnExit()
{
}
