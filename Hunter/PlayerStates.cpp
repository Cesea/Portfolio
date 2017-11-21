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



}


PlayerState * PlayerNormalState::Update(float deltaTime, const GameCommand &command)
{
	PlayerState *newState = nullptr;


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
	return nullptr;
}

void PlayerWarState::OnExit()
{
}

void PlayerDeadState::OnEnter()
{
}

PlayerState * PlayerDeadState::Update(float deltaTime, const GameCommand &command)
{
	return nullptr;
}

void PlayerDeadState::OnExit()
{
}
